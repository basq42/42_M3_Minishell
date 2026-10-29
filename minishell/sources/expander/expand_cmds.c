/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:06:58 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:07:51 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	splice_fields(t_cmd *scratch, char **fields)
{
	int	j;
	int	status;

	j = 0;
	status = 0;
	while (fields[j] && status == 0)
	{
		status = argv_append(scratch, fields[j]);
		j++;
	}
	free_strarr(fields);
	return (status);
}

static int	expand_argv(t_shell *sh, t_cmd *cmd)
{
	t_cmd	*scratch;
	char	**fields;
	int		i;

	scratch = cmd_new();
	if (!scratch)
		return (1);
	i = 0;
	while (cmd->argv[i])
	{
		fields = expand_word(sh, cmd->argv[i]);
		if (!fields || splice_fields(scratch, fields))
		{
			free_cmds(scratch);
			return (1);
		}
		i++;
	}
	free_strarr(cmd->argv);
	cmd->argv = scratch->argv;
	free(scratch);
	return (0);
}

static int	expand_redirs(t_shell *sh, t_cmd *cmd)
{
	t_redir	*redir;
	char	**fields;
	char	*new_file;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type != T_HEREDOC)
		{
			fields = expand_word(sh, redir->file);
			if (!fields)
				return (1);
			if (fields[0])
				new_file = ft_strdup(fields[0]);
			else
				new_file = ft_strdup("");
			free_strarr(fields);
			if (!new_file)
				return (1);
			free(redir->file);
			redir->file = new_file;
		}
		redir = redir->next;
	}
	return (0);
}

int	expand_cmds(t_shell *sh)
{
	t_cmd	*cmd;

	cmd = sh->cmds;
	while (cmd)
	{
		if (expand_argv(sh, cmd) || expand_redirs(sh, cmd))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
