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
 * !!! the builtin: exit wont return here and the saved fds stay open but kernel
 * closes them when process ends.
 * Returns: builtin command's exit status if ok, 1 on dup or redirection fail.
 * Caller: execute()
 * */
int	run_builtin_parent(t_shell *sh, t_cmd *cmd)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
	{
		print_error("dup", strerror(errno), NULL);
		close_if_open(saved_in);
		close_if_open(saved_out);
		return (1);
	}
	if (apply_redirs(cmd->redirs) == 0)
		status = run_builtin(sh, cmd->argv);
	else
		status = 1;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
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
