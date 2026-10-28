/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:23:42 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/28 17:28:01 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	HEREDOC ("<<") How it works:
 *
 *	"cat << EOF" reads lines from the user until a line is exactly EOF, then
 *	hands those lines to cat as stdin.
 *
 *	Order of stages in run_line():
 *		lexer -> parser -> heredoc_collect -> expander -> execute ->
 *		heredoc_cleanup.
 *	One command line can have multiple heredocs:
 *		cat << A << B | grep x << C
 *	In line with how bash handles, each heredoc of a command line are read
 *	before anything is expanded or run, right after the parser and each body is
 *	written into a temp file (/tmp/.minishell_hd_n) and the path is stored into
 *	redir->hd_path.
 *	Examples of bash behaviour:
 *		1. nothing runs until every heredoc of the command line is read.
 *				bash$ touch /tmp/case1 | cat << A | wc -l << B
 *				> first body
 *				> ^C
 *				bash$ ls /tmp/case1
 *				ls: cannot access '/tmp/case1': No such file or directory
 *
 *		2. several heredocs on one command are read in order, last one wins
 *				bash$ cat << A << B
 *				> first body
 *				> A
 *				> second body
 *				> B
 *				second body
 *
 *		3. heredocs of different commands in a pipeline are all read first.
 *				bash$ cat << A > /tmp/out_file | cat << B
 *				> first body of first cat
 *				> A
 *				> body of the second cat
 *				> B
 *				body for the second cat
 *				bash$ cat /tmp/out_file
 *				first body of first cat
 *	EACH heredoc is read by a forked CHILD PROCESS and NOT THE SHELL!:
 *		ctrl-c during a heredoc must stop heredoc and cancel the whole line.
 *		With defailt SIGINT behaviour the child just terminates, and the parent
 *		only sees 'killed by SIGINT' in waitpid() and cancels the line (130).
 * */
/*	MALLOC HERE
 *
 * Find a temp file name that does not exists, starting from:
 * /tmp/.minishell_hd_0 -> _1 then _2 and so on untill one is free. So *count is
 * shared by each heredoc of the command line, so each gets its own number and
 * moves it past every name that was tried.
 * WHY? => the name may already be taken by either another shell running in
 * parallel or a by a file left behind on a crashed run.
 *
 * ADDITION: use O_EXCL to check if name is taken to prevent truncating
 * a random file.
 * */
char	*hd_make_path(int *count)
{
	char	*number;
	char	*path;
	int		fd;

	while (1)
	{
		number = ft_itoa(*count);
		if (!number)
			return (NULL);
		path = ft_strjoin(HD_PREFIX, number);
		free(number);
		if (!path)
			return (NULL);
		(*count)++;
		fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd != -1 || errno != EEXIST)
		{
			if (fd != -1)
				close(fd);
			return (path);
		}
		free(path);
	}
}

/*
 * Parent's side of one heredoc: waits for the child that reads it, then turns
 * child's termination into status as per bash behaviour:
 * 	- SIGINT (ctrl-c): 			print a newline, return 130.
 * 	- exit with status != 0: 	error message printed by child, return 1.
 * 	- exit with status = 0:		the temo file is complete, return 0.
 * Caller: heredoc_one()
 * */
static int	hd_wait(pid_t pid)
{
	int	ws;

	waitpid(pid, &ws, 0);
	if (WIFSIGNALED(ws) && WTERMSIG(ws) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (130);
	}
	if (!WIFEXITED(ws) || WEXITSTATUS(ws) != 0)
		return (1);
	return (0);
}

/*
 * Reads one heredoc into its temp file through a child process. STEPS:
 * 	1. Call hd_make_path to chose a file name and store it in redir->hd_path
 * 	2. Ignore ctrl-c im the shell BEFORE FORK.
 * 		2.1. fork() copies the signal settings! Right after fork call, the child
 * 		has exactly the same signal handling. If the ignore came after fork(),
 * 		then both processes will ahve the same ctrl-c handler and a ctrl-c would
 * 		redraw "minishell$" in the middle f heredoc, and the child would run the
 * 		handler instead of terminating.
 * 	DEPENDS on signal/ implementation:
 * 		- here is assumed that PLACE_HOLDER_SET_SIGNALS_IGNORE_FUNC sets SIGINT
 * 		and SIGQUIT to SIG_IGN; if it DOES NOT then the prompt handler will be
 * 		still active in both processes.
 * 	3. fork(). The child then reads the lines.
 * 	4. The parent waits for the child and gets the status (hd_wait).
 * 	hd_path is stored before fork so the cleanup function can delete the file
 * 	even when the heredoc was terminated before finishing.
 * 	Returns: 0 on ok, 1 on any error, 130 on ctrl-c
 * 	Caller: heredoc_collect().
 * */
int	heredoc_one(t_shell *sh, t_redir *redir, int *count)
{
	pid_t	pid;

	redir->hd_path = hd_make_path(count);
	if (redir->hd_path == NULL)
	{
		print_error("out of memory", NULL, NULL);
		return (1);
	}
	set_signals_ignore();
	pid = fork();
	if (pid == -1)
	{
		print_error("fork", strerror(errno), NULL);
		return (1);
	}
	if (pid == 0)
		hd_child(sh, redir);
	return (hd_wait(pid));
}

/*
 * Entry Function: reads each heredoc line in order, before anything runs. Walks
 * every command and redirection of it.
 * 	"cat << A << B | wc << C" -> reads A, then B, then C.
 * 	Stops at first failed heredoc or ctrl-c, as the rest of the line must not
 * 	run as per bash.
 * 	Temp files created here are not deleted here: run_line always calls cleanup
 * 	function after, whatever the case.
 * 	Returns: 0 on ok, 1 on any error, 130 on ctrl-c.
 * 	Caller: run_line
 * */
int	heredoc_collect(t_shell *sh)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		count;
	int		status;

	count = 0;
	cmd = sh->cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC)
			{
				status = heredoc_one(sh, redir, &count);
				if (status != 0)
					return (status);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
