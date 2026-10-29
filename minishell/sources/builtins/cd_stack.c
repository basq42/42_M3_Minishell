/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_stack.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:03:35 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:04:49 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	grow_stack(char ***stack, char *copy)
{
	char	**grown;
	int		count;

	count = 0;
	while ((*stack)[count])
		count++;
	grown = malloc((count + 2) * sizeof(char *));
	if (!grown)
	{
		free(copy);
		return (1);
	}
	grown[count] = copy;
	grown[count + 1] = NULL;
	while (count > 0)
	{
		count--;
		grown[count] = (*stack)[count];
	}
	free(*stack);
	*stack = grown;
	return (0);
}

int	push_component(char ***stack, char *seg)
{
	char	*copy;

	copy = ft_strdup(seg);
	if (!copy)
		return (1);
	return (grow_stack(stack, copy));
}
