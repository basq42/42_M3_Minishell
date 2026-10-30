/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:22:30 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/30 09:29:23 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Opens the file of 1 redirection, with the flags its operator
 * needs as per bash:
 * 		<	read only.
 * 		>	write only, create if missing, existing->empty first (O_TRUNC).
 * 		>>	write only, create if missing, write at the end (O_APPEND).
 * 		<<	read_only, the temp file the HEREDOC already wrote (hd_path)
 * 	0644 = rw-r--r--	default permissions bash gives a file.
 * 	Returns: the new fd on ok, -1 on fail with errno set by open()
 * 	Caller: apply_redirs()
 * */
/*	NEW: For heredoc files: field check for 'ambiguous redirect' as bash does.
 *	+ unlink right after opening: file data stays alive as long as >=1 fd refers
 *	to it so unlinkin only removes name, hence file gets cleaned after process
 *	is done (no more fd referral to that file).
 * */
static int	open_redir(t_redir *redir)
{
	int	fd;

	if (redir->ambiguous)
	{
		print_error(redir->file, "ambiguous redirect", NULL);
		return (-2);
	}
	if (redir->type == T_IN)
		return (open(redir->file, O_RDONLY));
	if (redir->type == T_OUT)
		return (open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == T_APPEND)
		return (open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644));
	fd = open(redir->hd_path, O_RDONLY);
	if (fd != -1)
		unlink(redir->hd_path);
	return (fd);
}

/*
 * Apply each redirection of 1 command, one by one in source order.
 * For each:
 * 			1. open file
 * 			2. dup2 it:
 * 				2.1. if < or << then onto stdin
 * 				2.2. if > or >> then onto stdout
 * 			3. close the original fd.
 * STOPS at the FIRST instance of a FAILURE as per BASH:
 * 	"cat < missing > out": < fails first, so out file never created.
 * 	"echo X > a > b": 'a' is created empty, b recieves 'X'
 * 	After dup2(fd, target) close(fd) is called to prevent leaking into the
 * 	program started by execve().
 * 	Returns: 0 on ok, 1 on fail (error msg already printed)
 * 	Caller: child_process(), run_builtin_parent().
 * */
/*	NEW: now open_redir() returns -2 on ambiguous redirect error:
 *		export F="a b"; echo hi > $F
 * */
int	apply_redirs(t_redir *redir)
{
	int	fd;
	int	target;

	while (redir != NULL)
	{
		fd = open_redir(redir);
		if (fd == -2)
			return (1);
		if (fd == -1)
		{
			print_error(redir->file, strerror(errno), NULL);
			return (1);
		}
		if (redir->type == T_IN || redir->type == T_HEREDOC)
			target = STDIN_FILENO;
		else
			target = STDOUT_FILENO;
		dup2(fd, target);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
