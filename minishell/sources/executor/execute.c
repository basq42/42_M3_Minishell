/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:22:24 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:31:05 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * For when a BUILTIN command needs to run (inside shell, no fork) with its
 * redirections -> STEPS:
 * 			1. Save the real STDIN and STDOUT with dup().
 * 			2. Apply the redirections so: fd0 and fd1 may now point to files.
 * 			3. Run the bultin if and only if redirections worked.
 * 											if not status 1.
 * 			4. Revert back the saved stdin and stdout with dup2 + close copies.
 * Why this architecture?: As the shell keeps running after the bultin command's
 * process finish, so without saving and reverting the stdin and stdout, after
 * a case like: "pwd > f" every later prompt and output would go into f.
 *

* DEPENDABLE ON RUN_BUILTIN_FUNC()specifically the exit builtin:
 * 		- if it calls shell_exit then RUN_BUILTIN_FUNC() wont return here and
 * 		the 2 saved fds stay open and the kernel then closes them at exit.
 * 		- if it will return a status and let main end the shell then STEP 4 runs
 * 		normally and saved fds are closed here.
 *
 * !!! the builtin: exit wont return here and the saved fds stay open but kernel
 * closes them when process ends.
 * 			|	23.10.2026 FIX!!!
 * 			|=> Well, apperantly not :'). FIX: add two variables to the t_shell
 * 			struct to track the shared fds, so shell_exit can close them.
 * 			So disregard the above "DEPENDABLE", now BUILTIN will NEVER RETURN
 * 			HERE and instead HAS TO CALL shell_exit, if not the 2 copies will
 * 			stay open.
 * 			VERY CRUCIAL: ANY FUNCTION that CLOSES a COPY OF ANY FD, has to set
 * 			it to -1, otherwise lose track:
 * 				echo hi		-> saved_in = 3 and then closed
 * 				cat | wc	-> as kernel hands out low fd numbers, the
 * 				pipeline's may have fd3 in one end. => shell_exit my double
 * 				close old fds, or a wrong close a new one.
 *
 *
 * Returns: builtin command's exit status if ok, 1 on dup or redirection fail.
 * Caller: execute()
 * */
static int	run_builtin_parent(t_shell *sh, t_cmd *cmd)
{
	int	status;

	sh->saved_in = dup(STDIN_FILENO);
	sh->saved_out = dup(STDOUT_FILENO);
	if (sh->saved_in == -1 || sh->saved_out == -1)
	{
		print_error("dup", strerror(errno), NULL);
		close_if_open(sh->saved_in);
		close_if_open(sh->saved_out);
		sh->saved_in = -1;
		sh->saved_out = -1;
		return (1);
	}
	if (apply_redirs(cmd->redirs) == 0)
		status = run_builtin(sh, cmd->argv);
	else
		status = 1;
	dup2(sh->saved_in, STDIN_FILENO);
	dup2(sh->saved_out, STDOUT_FILENO);
	close(sh->saved_in);
	close(sh->saved_out);
	sh->saved_in = -1;
	sh->saved_out = -1;
	return (status);
}

/*
 * ENTRY point of the executor: runs ALL the commands of sh->cmds
 * TWO PATHS:
 * 		1. if ONE command that is builtin ('cd /tmp', 'export X=2', 'exit'...)
 * 			=> call run_builtin_parent
 * 		2. All else (external command, pipeline, etc)
 * 			=> call run_pipeline with one child per command.
 * WHY keep a lone builtin in a parent: As per BASH:
 * 			=> cd, export, exit etc change the shell itself. If this was done by
 * 			a child process, then the change would wanish with the child's exit.
 * 			=> Differently: inside a pipeline ('cd /tmp | cat') bash also runs
 * 			them in child processes, and the change is lost in bash too.
 * Returns: the exit status of the line that becomes: $?
 * Caller: run_line().
 * */
int	execute(t_shell *sh)
{
	t_cmd	*cmd;

	cmd = sh->cmds;
	if (cmd == NULL)
		return (0);
	if (cmd->next == NULL && cmd->argv != NULL && cmd->argv[0] != NULL
		&& is_builtin(cmd->argv[0]))
		return (run_builtin_parent(sh, cmd));
	return (run_pipeline(sh));
}
