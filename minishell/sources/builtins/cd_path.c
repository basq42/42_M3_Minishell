/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:05:01 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:06:02 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_segment(char ***stack, char *seg)
{
	int	last;

	if (ft_strncmp(seg, ".", 2) == 0)
		return (0);
	if (ft_strncmp(seg, "..", 3) == 0)
	{
		last = 0;
		while ((*stack)[last])
			last++;
		if (last > 0)
		{
			free((*stack)[last - 1]);
			(*stack)[last - 1] = NULL;
		}
		return (0);
	}
	return (push_component(stack, seg));
}

static int	fill_stack(char ***stack, char *path)
{
	char	**segs;
	int		i;
	int		status;

	segs = ft_split(path, '/');
	if (!segs)
		return (1);
	i = 0;
	status = 0;
	while (segs[i] && status == 0)
		status = apply_segment(stack, segs[i++]);
	free_strarr(segs);
	return (status);
}

static char	*join_stack(char **stack)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (stack[i] && result)
	{
		result = join_free(result, ft_strdup("/"));
		result = join_free(result, ft_strdup(stack[i]));
		i++;
	}
	if (result && !result[0])
	{
		free(result);
		result = ft_strdup("/");
	}
	return (result);
}

static int	walk_target(char ***stack, char *target, int status)
{
	char	**segs;
	int		i;

	segs = ft_split(target, '/');
	if (!segs || status != 0)
	{
		free_strarr(segs);
		return (1);
	}
	i = 0;
	while (segs[i] && status == 0)
		status = apply_segment(stack, segs[i++]);
	free_strarr(segs);
	return (status);
}

char	*build_new_pwd(char *old_pwd, char *target)
{
	char	**stack;
	char	*result;
	int		status;

	stack = malloc(sizeof(char *));
	if (!stack)
		return (NULL);
	stack[0] = NULL;
	status = 0;
	if (target[0] != '/' && old_pwd)
		status = fill_stack(&stack, old_pwd);
	status = walk_target(&stack, target, status);
	if (status != 0)
	{
		free_strarr(stack);
		return (NULL);
	}
	result = join_stack(stack);
	free_strarr(stack);
	return (result);
}
