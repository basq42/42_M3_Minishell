/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:04:40 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 15:32:52 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * on invalid KEY -> print_ident_error to get bash format with ` '
 * */
static int	export_one(t_shell *sh, char *arg)
{
	char	*eq;
	char	*key;
	int		status;

	eq = ft_strchr(arg, '=');
	if (eq)
		key = ft_substr(arg, 0, eq - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		print_ident_error("export", arg);
		free(key);
		return (1);
	}
	if (eq)
		status = env_set(&sh->env, key, eq + 1);
	else
		status = env_set(&sh->env, key, NULL);
	free(key);
	if (status != 0)
		print_error("out of memory", NULL, NULL);
	return (status);
}

int	bt_export(t_shell *sh, char **argv)
{
	int	i;
	int	ret;

	if (!argv[1])
		return (print_export(sh));
	i = 1;
	ret = 0;
	while (argv[i])
	{
		if (export_one(sh, argv[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
