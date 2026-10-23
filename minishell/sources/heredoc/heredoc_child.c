/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:24:05 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:24:07 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	MALLOC HERE
 * ctrl-d on a empty "> " prompt ends the heredoc early, and what was typed so
 * far is accepted but only prints warning (as per bash).

*  minishell: warning: here-document delimited by end-of-file (wanted `EOF')
 * message is built with 2 ft_strjoin calls. if one fails, a shorter message is
 * printed but the heredoc still succeeds.
 * Returns: always 0
 * Caller: hd_read_loop().
 * */
static int	hd_eof_warning(char *dlm)
{
	char	*start;
	char	*msg;

	start = ft_strjoin("here-document delimited by end-of-file (wanted `", dlm);
	msg = NULL;
	if (start)
		msg = ft_strjoin(start, "')");
	free(start);
	if (!msg)
		print_error("warning", "here-document delimited by end-of-file", NULL);
	else
		print_error("warning", msg, NULL);
	free(msg);
	return (0);
}

/*
 * Reads the heredoc body line by line with readline("> "), and writes each line
 * + the newline readline() removed, into the temp file (fd).
 * Stop Conditions:
 * 	- a line is EXACTLY the delimiter itself	-> done, return (0);
 * 	- ctrl-d (readline returns null)			-> warning, return (0);
 * 	- out of memory while EXPANDING				-> return (1);
 * To achieve 'EXACTLY': strncmp compares strlen(dlm) + 1 chars, so the '\0' is
 * also compared. So, for example, "EOFX" or "EO" dont end an "EOF" heredoc.
 * expand == 1 -> each line goes through PLACE_HOLDER_EXPAND_STRING_FUNC().
 * So DEPENDS on implementation:
 * 	- if it expands $VAR and $? and leaves quotes alone: same body as bash
 * 	(quotes inside heredoc body never removed).
 * 	- if it returns NULL: treated as out of memory and the heredoc fails.
 * The lines are not stored as history.
 * Returns: 0 on ok, 1 on oom.
 * Caller: hd_child.
 * */
static int	hd_read_loop(t_shell *sh, int fd, char *dlm, int expand)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (hd_eof_warning(dlm));
		if (ft_strncmp(line, dlm, ft_strlen(dlm) + 1) == 0)
		{
			free(line);
			return (0);
		}
		if (expand)
		{
			expanded = PLACE_HOLDER_EXPAND_STRING_FUNC(sh, line);
			free(line);
			if (!expanded)
				return (1);
			line = expanded;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

/*
 * All heredoc child is here. Never returns, rather ends in shell_exit, whcih
 * frees the child's copy of the shell's memory.
 * STEPS:
 * 	1. set_signals_heredoc: ctrl-c kills this child (default SIGINT)
 * 							ctrl-\ does nothing.
 * 			|_> Child must set its own as shell ignores SIGINT and then calls
 * 			fork(). NEver calls execve.
 * 	2. Create the temo file, readable and writable by ONLY ITS OWNER (0600).
 * 	3. Delimiter as typed may hold quotes: << "EOF" or << 'E'OF. In this case
 * 	the line that ends heredoc is the delimiter WITHOUT the quotes (EOF).
 * 	4. Expansion Rule per bash: if delimiter had any QUOTE, the body is kept
 * 	literally, otherwise $VAR and $? in it are EXPANDED.
 * 			<< EOF		-> "$HOME" in the body becomes: /home/...
 * 			<< 'EOF'	-> "$HOME" stays as is.
 * 		The delimiter itself is never expanded:
 * 			<< $USER waits for the literal line: $USER
 * 	5. Read the body, close, exit with its status.
 * 	DEPENDS ON signal/ and expander/ implementation:
 * 		- set_signals_heredoc: if it sets SIGINT to SIG_DFL, ctrl-C kills this
 * 		child and hd_wait returns 130; if it installs a handler instead,
 * 		readline keeps waiting ctrl-c and does not cancel.
 * 		- remove_quotes: if it removes ' and " without expanding, << "EOF" ends
 * 		on the line EOF, as per bash. If it returns NULL, heredoc fails with
 * 		status 1.
 * 	CALLER: heredoc_one (in the child right after fork)
 * */
void	hd_child(t_shell *sh, t_redir *redir)
{
	int		fd;
	char	*dlm;
	int		expand;
	int		status;

	PLACE_HOLDER_SET_SIGNALS_HEREDOC_FUNC();
	fd = open(redir->hd_path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		print_error(redir->hd_path, strerror(errno), NULL);
		shell_exit(sh, 1);
	}
	dlm = PLACE_HOLDER_REMOVE_QUOTES_FUNC(redir->file);
	if (!dlm)
	{
		close(fd);
		shell_exit(sh, 1);
	}
	expand = 1;
	if (ft_strchr(redir->file, '\'') || ft_strchr(redir->file, '"'))
		expand = 0;
	status = hd_read_loop(sh, fd, dlm, expand);
	close(fd);
	free(dlm);
	shell_exit(sh, status);
}
