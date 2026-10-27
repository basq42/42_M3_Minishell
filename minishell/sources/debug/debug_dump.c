/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_dump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 12:00:00 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/27 12:00:00 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* DEBUG BRANCH ONLY: "[tok] [cat] [<<] [EOF] [|] ..." Caller: run_line. */
void	dbg_tokens(t_token *token)
{
	if (!DEBUG)
		return ;
	dbg_put("[tok]");
	while (token)
	{
		dbg_put(" [");
		if (token->type == T_WORD)
			dbg_put(token->value);
		else
			dbg_put(token_text(token->type));
		dbg_put("]");
		token = token->next;
	}
	dbg_put("\n");
}

static void	dump_argv(char **argv)
{
	int	i;

	dbg_put("argv:");
	i = 0;
	while (argv && argv[i])
	{
		dbg_put(" [");
		dbg_put(argv[i]);
		dbg_put("]");
		i++;
	}
}

static void	dump_redirs(t_redir *redir)
{
	dbg_put("  redirs:");
	if (!redir)
		dbg_put(" none");
	while (redir)
	{
		dbg_put(" [");
		dbg_put(token_text(redir->type));
		dbg_put(" ");
		dbg_put(redir->file);
		if (redir->hd_path)
		{
			dbg_put(" -> ");
			dbg_put(redir->hd_path);
		}
		dbg_put("]");
		redir = redir->next;
	}
}

/* "[cmd 1 parsed] argv: [grep] ["$X"]  redirs: [> out.txt]" per command */
void	dbg_cmds(const char *when, t_cmd *cmd)
{
	int	n;

	if (!DEBUG)
		return ;
	n = 1;
	while (cmd)
	{
		dbg_put("[cmd ");
		ft_putnbr_fd(n, STDERR_FILENO);
		dbg_put(" ");
		dbg_put(when);
		dbg_put("] ");
		dump_argv(cmd->argv);
		dump_redirs(cmd->redirs);
		dbg_put("\n");
		cmd = cmd->next;
		n++;
	}
}

/*
 * Fd leak detector: lists every descriptor above 2 that is open in THIS
 * process (fstat succeeds only on an open fd). In the shell, after a line,
 * the answer should be "none". Caller: run_line.
 * */
void	dbg_fds(void)
{
	struct stat	st;
	int			fd;
	int			found;

	if (!DEBUG)
		return ;
	dbg_put("[fds] open above 2:");
	found = 0;
	fd = 3;
	while (fd < 64)
	{
		if (fstat(fd, &st) == 0)
		{
			dbg_put(" ");
			ft_putnbr_fd(fd, STDERR_FILENO);
			found = 1;
		}
		fd++;
	}
	if (!found)
		dbg_put(" none");
	dbg_put("\n");
}
