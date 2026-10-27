/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:00:00 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/27 12:00:00 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * DEBUG BRANCH ONLY. Every debug line goes to stderr (fd 2), so it never ends
 * up in a pipe or a redirected file. DEBUG is set in minishell.h; build with
 * -DDEBUG=0 to silence everything without removing the calls.
 * Several write() calls per line: lines of two processes may interleave.
 * */
void	dbg_put(const char *s)
{
	if (s)
		write(STDERR_FILENO, s, ft_strlen(s));
	else
		write(STDERR_FILENO, "NULL", 4);
}

/* "[tag] msg" */
void	dbg(const char *tag, const char *msg)
{
	if (!DEBUG)
		return ;
	dbg_put("[");
	dbg_put(tag);
	dbg_put("] ");
	dbg_put(msg);
	dbg_put("\n");
}

/* "[tag] msg n" */
void	dbg_num(const char *tag, const char *msg, int n)
{
	if (!DEBUG)
		return ;
	dbg_put("[");
	dbg_put(tag);
	dbg_put("] ");
	dbg_put(msg);
	dbg_put(" ");
	ft_putnbr_fd(n, STDERR_FILENO);
	dbg_put("\n");
}

/* "[tag] msg s" (s may be NULL) */
void	dbg_str(const char *tag, const char *msg, const char *s)
{
	if (!DEBUG)
		return ;
	dbg_put("[");
	dbg_put(tag);
	dbg_put("] ");
	dbg_put(msg);
	dbg_put(" ");
	dbg_put(s);
	dbg_put("\n");
}
