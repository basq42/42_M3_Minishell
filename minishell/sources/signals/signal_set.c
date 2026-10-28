/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_set.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 17:20:46 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/28 17:37:33 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* CALLING:
 *  set_sig_int_quit(SIG_IGN, SIG_IGN) -> shell while children running
 * 	set_sig_int_quit(SIG_DFL, SIG_DFL) -> child before execve
 * 	set_sig_int_quit(SIG_DFL, SIG_IGN) -> heredoc read
 * 	In More Detail:
 * THE FOUR SETTERS (1 is in signal.c): which setting, called where, and why.
 * 	(handler = run a function, IGN = ignore, DFL = default: terminate)
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
 */
void	set_signals_ignore(void)
{
	set_sig_int_quit(SIG_IGN, SIG_IGN);
}

void	set_signals_default(void)
{
	set_sig_int_quit(SIG_DFL, SIG_DFL);
}

void	set_signals_heredoc(void)
{
	set_sig_int_quit(SIG_DFL, SIG_IGN);
}
