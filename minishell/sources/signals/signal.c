/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 15:49:46 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/28 17:20:26 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * !! SIGNALS !!
 *    => signals are asynchronous notifications from the kernel (or another
 *    process) that interrupts the program's flow. As the handler runs in the
 *    same process, on the same thread, at an ARBITRARY POINT: is possible in
 *    middle of a malloc, printf, loop,... So a handler must: be minimalist in
 *    actions so: store signal number for main to act on it after readline
 *    returns $? = 130.
 * Why this type for global:
 * 	1. KERNEL: a signal can interrupt main between ANY TWO INSTRUCTIONS and runs
 * 	the handler with only the SIGNAL NUMBER as ARGUMENT;
 * 		=> Convention: handler records only the signal in a global; main routine
 * 		checks it later (after readline returns) and sets $?=130;
 * 	2. COMPILER: optimeses the main routine with assuming that main has been set
 * 	to be the only function editing it's variables.
 * 		=> [volatile] informs the compiler not to optimise access to variable,
			so
 * 		every read/write goes to the actual memory location instead of a virtual
 * 		copy left in the register, or caches. This way, variables that are
 * 		expected to be edited outside of the program's normal flow
 * 		(hardware regs, flags set by interrupt routines, data shared with sig
 * 		handlers...). Without this, an compiler optimised main may keep reading
 * 		an old/cached copy instead of accessing handler's write.
 * 		=> [sig_atomic_t] a type int that is read/written in ONE indivisable
 * 		access so a signal can never land midway through a write to then leave a
 * 		semi-written value. !! ONLY PLAIN ASSIGNMENT IS SAFE:
 * 			g_signal++ is read + change + write (a signal can split arbitraryly)
 * For readline(rl) specific info refer to main.c
 * signal() is not portable -> use sigaction()!
 * */
volatile sig_atomic_t	g_signal = 0;

/*	!! rl_* functions are NOT ASYNC-SIGNAL-SAFE!!! but no issues as the setup
 *	assures that no signal arrives while readline is not waiting for input.
 *	*/
static void	prompt_sigint_handler(int signal)
{
	g_signal = signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*	signal.h: struct sigaction: sa_handler, sa_mask, sa_flags
 *	struct sigaction dictates (as type of sa_handler): void(*)(int);
 * */
void	set_sig_int_quit(void (*on_sigint)(int), void (*on_sigquit)(int))
{
	struct sigaction	sig_act;

	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_flags = 0;
	sig_act.sa_handler = on_sigint;
	sigaction(SIGINT, &sig_act, NULL);
	sig_act.sa_handler = on_sigquit;
	sigaction(SIGQUIT, &sig_act, NULL);
}

void	set_signals_prompt(void)
{
	set_sig_int_quit(prompt_sigint_handler, SIG_IGN);
}
/*
 * HOW TO CALL THE FUNCTIONS:
 * 	set_sig_int_quit(prompt_sigint_handler, SIG_IGN) -> prompt
 * 	set_sig_int_quit(SIG_IGN, SIG_IGN) -> shell while children running
 * 	set_sig_int_quit(SIG_DFL, SIG_DFL) -> child before execve
 * 	set_sig_int_quit(SIG_DFL, SIG_IGN) -> heredoc read
 * 	In More Detail:
 * THE FOUR SETTERS: which setting, called where, and why.
 * 	(handler = run a function, IGN = ignore, DFL = default: terminate)
 *
 * 1. SET_SIGNALS_PROMPT: SIGINT -> prompt handler, SIGQUIT -> IGN
 * 	Called by: main, at the top of EVERY loop iteration.
 * 	ctrl-C redraws an empty prompt ($? = 130); ctrl-\ does nothing (subject).
 * 	Every iteration because run_pipeline and heredoc_one switch the shell
 * 	to IGN and nothing switches it back.
 *
 * 2. SET_SIGNALS_IGNORE: SIGINT -> IGN, SIGQUIT -> IGN
 * 	Called by: run_pipeline and heredoc_one, BEFORE fork.
 * 	ctrl-C reaches the shell AND its children (foreground group): the
 * 	shell must survive to wait for them. Before fork, so no child ever
 * 	starts with the prompt handler.
 *
 * 3. SET_SIGNALS_DEFAULT: SIGINT -> DFL, SIGQUIT -> DFL
 * 	Called by: child_process, as its first line.
 * 	The child inherited IGN, and an ignored signal stays ignored after
 * 	execve: without this, ctrl-C could not stop cat. With it: 130 / 131.
 *
 * 4. SET_SIGNALS_HEREDOC: SIGINT -> DFL, SIGQUIT -> IGN
 * 	Called by: hd_child, as its first line.
 * 	ctrl-C must kill the heredoc reader so the line is cancelled (130 in
 * 	hd_wait). It never calls execve, so it must reset SIGINT itself.
 * 	ctrl-\ does nothing, as in bash.
 * */
