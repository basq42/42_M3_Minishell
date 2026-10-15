#include "minishell.h"

/*
 * the ONLY way the program ENDs: free everything the shell owns, then exit with
 * status and never return.
 * 	- env_free:			the environment list.
 * 	- free_cmds:		the commands of the current line, could be NULL.
 * 	- rl_clear_history:	readline's history list.
 * 	Used in EVERY CHILD: after a builtin, or failed redirection or command or at
 * 	the end of a heredoc.
 * 	WHY: fork gives the child a full copy of parent's memory. If a child exit()
 * 	without freeing then leak.
 * 	DOES NOT DELETE heredoc temp files as a child exiting must not remove a file
 * 	that another child still needs (refer to heredoc_cleanup comments).
 *
 * 	CALLER: child_process, exec_external, exec_fail, hd_child
		+ main and builtins
 * 		- If main will call shell_exit on ctrl-D (in which case readline returns
 * 		NULL): the shell ends with $? and everything freed.
 * 		- If the exit builtin calls shell_exit: same, with exit's status.
 * 		- If either calls exit() directly instead: env and cmds WONT BE FREED
 * 		HENCE CAUSING LEAKS!
 * */
void	shell_exit(t_shell *sh, int status)
{
	env_free(sh->env);
	free_cmds(sh->cmds);
	rl_clear_history();
	exit(status);
}
