#include "minishell.h"

/* ==========================================================================
 * TEST STUB - NOT PART OF THE SUBMISSION - DELETE stubs/ BEFORE SUBMITTING
 * Stands in for Bahtiyor's main.c until the real one lands.
 * Built only by "make stubs" (binary: minishell_stubs), never by "make".
 * Follows the contract in the implementation guide, Section 4.
 * ========================================================================== */

/*
 * The prompt loop:
 *	1. Set the prompt's signals (ctrl-C redraws the prompt, ctrl-\ ignored).
 *	2. readline shows "minishell$ " and returns the typed line.
 *	3. If ctrl-C was pressed while typing, $? becomes 130. This check comes
 *		BEFORE the ctrl-D check, so ctrl-C then ctrl-D exits with 130.
 *	4. NULL means ctrl-D on an empty line: print "exit", end the shell.
 *	5. A non-empty line goes to the history and to run_line.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	char	*line;

	(void)argc;
	(void)argv;
	if (shell_init(&sh, envp) != 0)
	{
		print_error("out of memory", NULL, NULL);
		return (1);
	}
	while (1)
	{
		PLACE_HOLDER_SET_SIGNALS_PROMPT_FUNC();
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			sh.last_status = 130;
			g_signal = 0;
		}
		if (line == NULL)
		{
			write(STDERR_FILENO, "exit\n", 5);
			shell_exit(&sh, sh.last_status);
		}
		if (line[0] != '\0')
		{
			add_history(line);
			run_line(&sh, line);
		}
		free(line);
	}
}
