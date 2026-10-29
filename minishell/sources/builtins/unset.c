/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:05:08 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 15:42:17 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Similar to export invalid name error issue: unset A-B prints an error and
 * returns 1 while BASH ignores it silently and returns 0;
 * FIX: an invalid name never exists in evnironment, so respectfully env_unset
 * will find nothing on invalid name.
 * */
int	bt_unset(t_shell *sh, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		env_unset(&sh->env, argv[i]);
		i++;
	}
	return (0);
}
/* OLD VERSION
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
}*/
