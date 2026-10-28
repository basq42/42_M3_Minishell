/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:20:04 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/28 17:27:40 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Prints "minishell: a: b" and exits the process with status.
 * Never returns: CHILD ONLY and in the parent it would kill the shell!!
 * Called by: exec_external, find_in_path;
 */
void	exec_fail(t_shell *sh, char *a, char *b, int status)
{
	print_error(a, b, NULL);
	shell_exit(sh, status);
}

/*
 * Finds, checks and execve an external command.
 * Never returns: exits 127/126/1 with a bash style failure message.
 * CHILD ONLY. Called by: child_process.
 * */
static void	exec_external(t_shell *sh, char **argv)
{
	char	*path;
	char	**envp;
	int		status;

	if (argv[0][0] == '\0')
		exec_fail(sh, "", "command not found", 127);
	if (ft_strchr(argv[0], '/'))
		path = ft_strdup(argv[0]);
	else
		path = find_in_path(sh, argv[0]);
	if (!path)
		exec_fail(sh, "out of memory", NULL, 1);
	status = check_exec_path(argv[0], path);
	if (status != 0)
	{
		free(path);
		shell_exit(sh, status);
	}
	envp = env_to_array(sh->env);
	if (envp)
		execve(path, argv, envp);
	print_error(argv[0], strerror(errno), NULL);
	free(path);
	free_strarr(envp);
	shell_exit(sh, 126);
}

/*
 * Every forked pipeline child:
 * 			1. connects pipe ends to stdin and stdout,
 * 			2. applies the command's redirections (which OVERRIDE the pipe),
 * 			3. then runs a bultin or an external command.
 * A FAILED redir exits 1. A command with only redirs: ("> f") has
 * argv[0] == NULL and exits 0 after the files are opened.
 * WHY set the signals to default as first thing before execve:
 * 	run_pipeline made the shell ignore SIGINT/SIGQUIT before FORK and this child
 * 	got a copy of that. What execve does with a signal:
 * 		- ignored	-> stays IGNORED in the new program.
 * 		- a handler	-> reset to default with handler's code scrapped.
 * 		- default	-> stays default.
 * 	So without the reset, ignore would pass through execve to the new program:
 * 		"cat" would ignore ctrl-c
 * 	BUT this way, ctrl-c kills "cat" process, and wait_children reports 130 for
 * 	ctrl-c or 131 for ctrl-\
 *
 * 	SO the implementation of SET_SIGNALS_DEFAULT has to set SIGINT and SIGQUIT
 * 	to SIG_DFL, if not then the ctrl-c wont stop child processes.
 * No return. Called by: spawn_one, right after fork() call.
 */
void	child_process(t_shell *sh, t_cmd *cmd, t_pipe *p)
{
	set_signals_default();
	if (p->prev_read != -1)
	{
		dup2(p->prev_read, STDIN_FILENO);
		close(p->prev_read);
	}
	if (p->fd[1] != -1)
	{
		dup2(p->fd[1], STDOUT_FILENO);
		close(p->fd[1]);
	}
	if (p->fd[0] != -1)
		close(p->fd[0]);
	if (apply_redirs(cmd->redirs) != 0)
		shell_exit(sh, 1);
	if (cmd->argv == NULL || cmd->argv[0] == NULL)
		shell_exit(sh, 0);
	if (PLACE_HOLDER_IS_BUILTIN_FUNC(cmd->argv[0]))
		shell_exit(sh, PLACE_HOLDER_RUN_BUILTIN_FUNC(sh, cmd->argv));
	exec_external(sh, cmd->argv);
}

/*	PSEUDO CODE FOR EXTERNAL FUNCTIONS CALLED HERE
 *	apply_redirs(r)           -> redirect.c   open each redir in order,
		dup2 onto 0/1, close; 0 ok, 1 fail
is_builtin(name)
	-> teammate     1 for echo cd pwd export unset env exit
run_builtin(sh, argv)     -> teammate     runs it, returns status
set_signals_default()     -> teammate     SIGINT/SIGQUIT back to SIG_DFL
env_to_array(env)         -> env_array.c  "KEY=value" char ** for execve,
	NULL on oom
shell_exit(sh, status)    -> shell_exit.c env_free, free_cmds, rl_clear_history,
	exit
 */
