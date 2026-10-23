/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:25:24 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:26:17 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * MALLOC HERE!! -> allocates one EMPTY command.
 * The executor checks: cmd->argv[0] == NULL to detect a command made of
 * only redirections, such as "> f"
 * Returns: new command on ok, NULL on oom.
 * Caller: add_comand()
 * */
t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *));
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->argv[0] = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

/*
 * To keep the order of the pipeline, commands are appended at the end of list.
 * Same pattern as in tokens (token_add_back)
 * Calles: add_comand
 * */
void	cmd_add_back(t_cmd **list, t_cmd *cmd)
{
	t_cmd	*last;

	if (!*list)
	{
		*list = cmd;
		return ;
	}
	last = *list;
	while (last->next != NULL)
		last = last->next;
	last->next = cmd;
}

/*
 * Appends a COPY of word at the end of cmd->argv
 * !! An array cant grow in palce so:
 * 		1. Count current words
 * 		2. Allocate a new array with 1 more word + null
 * 		3. Append new word + NULL at the end
 * 			3.2. Then copy old pointers from last one to the first
 * 		4. Free old array
 * word is copied as the token list is freed right after parsing.
 * Returns: 0 on ok, 1 on oom with cmd->argv left unchanged.
 * Caller: build_cmds()
 * */
int	argv_append(t_cmd *cmd, char *word)
{
	char	**new_argv;
	int		count;

	count = 0;
	while (cmd->argv[count] != NULL)
		count++;
	new_argv = malloc((count + 2) * sizeof(char *));
	if (!new_argv)
		return (1);
	new_argv[count] = ft_strdup(word);
	if (!new_argv[count])
	{
		free(new_argv);
		return (1);
	}
	new_argv[count + 1] = NULL;
	while (count > 0)
	{
		count--;
		new_argv[count] = cmd->argv[count];
	}
	free(cmd->argv);
	cmd->argv = new_argv;
	return (0);
}

/*
 * MALLOC HERE!! -> allocates one redir with a COPY of file.
 * file can be either the file name or the raw heredoc delimiter: <<
 * hd+path stays NULL until heredoc stage fills it.
 * Returns: new redirection on ok, NULLL on oom.
 * Caller: redor_append
 * */
static t_redir	*redir_new(t_type type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->type = type;
	redir->hd_path = NULL;
	redir->next = NULL;
	return (redir);
}

/*
 * Adds a redir at the END of cmd->redirs
 * Why this order?
 * 		=> redirs have to be applied in the order they were inputted:
 * 		Ex: "echo x > a > b" must first create 'a' then write into 'b'
 * Returns: 0 on ok, 1 on oom.
 * Caller: build_cmds()
 * */
int	redir_append(t_cmd *cmd, t_type type, char *file)
{
	t_redir	*redir;
	t_redir	*last;

	redir = redir_new(type, file);
	if (!redir)
		return (1);
	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return (0);
	}
	last = cmd->redirs;
	while (last->next)
		last = last->next;
	last->next = redir;
	return (0);
}
