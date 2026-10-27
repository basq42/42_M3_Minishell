/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:23:50 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:24:08 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * DELETES every heredoc temp file of the line from the disk (unlink), frees its
 * path and sets it to NULL, so it wont be freed twice.
 * Called ONCE PER LINE, by run_line(), AFTER all children FINISHED, or after a
 * heredoc ERROR or a ctrl-c in which case files may be semi written.
 *
 * All deletion is here and only here: and not in, for ex, free_redirs:
 * 	All children run free_redirs when it exits. In the case of:
 * 		"cat << A | cat << B"
 * 	the first child could finish and delete B's file before the second child has
 * 	opened it. hence delegating the task to the parent, that is the only process
 * 	that 'knows' when no process needs the files any longer.
 * 	Caller: run_line();
 * */
void	heredoc_cleanup(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->hd_path)
			{
				dbg_str("clean", "unlink", redir->hd_path);
				unlink(redir->hd_path);
				free(redir->hd_path);
				redir->hd_path = NULL;
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
