#include "minishell.h"

/*
 * Fills the t_shell struct ONCE, at startup, BEFORE the prompt loop.
 * 	- cmds = NULL:		no command line has been parsed yet.
 * 	- last_status = 0:	$? is 0 when shell starts as per bash.
 * 	- env:				the shell's own copy of its environment, built from
 * 	envp, so export / unset can change it (getenv only sees the og)
 * 	Why the extra check on envp: in the case of:
 * 		"env -i ./minishell" starts the shell with an EMPTY environment,
 * 		env_init then correctly returns NULL which MUST NOT mistaken with malloc
 * 		failure. Null is only an error if envp had at LEAST ONE ENTRY to copy.
 * 	Returns: 0 on ok, 1 on oom withouth any message printed.
 * 	CONDITIONAL ON IMPLEMENTATION:
 * 		- if main checks the return: on 1, should print "out of memory" and
 * 		return 1 before the prompt loop.
 * 		- if you chose to ignore in main: the shell would start with an empty or
 * 		partial environment silently!
 * */
int	shell_init(t_shell *sh, char **envp)
{
	sh->cmds = NULL;
	sh->last_status = 0;
	sh->env = PLACE_HOLDER_ENV_INIT(envp);
	if (sh->env == NULL && envp != NULL && envp[0] != NULL)
		return (1);
	return (0);
}
