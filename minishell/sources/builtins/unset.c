/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:05:08 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:06:16 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bt_unset(t_shell *sh, char **argv)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			print_error("unset", argv[i], "not a valid identifier");
			ret = 1;
		}
		else
			env_unset(&sh->env, argv[i]);
		i++;
	}
	return (ret);
}
