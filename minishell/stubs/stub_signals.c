#include "minishell.h"

/* ==========================================================================
 * TEST STUB - NOT PART OF THE SUBMISSION - DELETE stubs/ BEFORE SUBMITTING
 * Stands in for Bahtiyor's signals/ until the real one lands.
 * Built only by "make stubs" (binary: minishell_stubs), never by "make".
 * Follows the contract in the implementation guide, Section 12.
 * ========================================================================== */

/* The only global: the number of the last signal received at the prompt. */
volatile sig_atomic_t	g_signal = 0;

/*
 * ctrl-C at the prompt: remember it, go to a new line, show an empty
 * prompt again. Only touches g_signal and readline's display.
 */
static void	prompt_sigint_handler(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* Sets what SIGINT (ctrl-C) and SIGQUIT (ctrl-\) do, with sigaction. */
static void	set_int_and_quit(void (*on_int)(int), void (*on_quit)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = on_int;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = on_quit;
	sigaction(SIGQUIT, &sa, NULL);
}

/* At the prompt: ctrl-C redraws the prompt, ctrl-\ does nothing. */
void	PLACE_HOLDER_SET_SIGNALS_PROMPT_FUNC(void)
{
	set_int_and_quit(prompt_sigint_handler, SIG_IGN);
}

/* The shell while children run: both ignored, the shell survives. */
void	PLACE_HOLDER_SET_SIGNALS_IGNORE_FUNC(void)
{
	set_int_and_quit(SIG_IGN, SIG_IGN);
}

/* A child before execve: both back to default, so ctrl-C kills cat. */
void	PLACE_HOLDER_SET_SIGNALS_DEFAULT_FUNC(void)
{
	set_int_and_quit(SIG_DFL, SIG_DFL);
}

/* A heredoc child: ctrl-C kills it, ctrl-\ does nothing. */
void	PLACE_HOLDER_SET_SIGNALS_HEREDOC_FUNC(void)
{
	set_int_and_quit(SIG_DFL, SIG_IGN);
}
