/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_b.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:03:31 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:04:48 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *name)
{
	if (!name)
		return (0);
	if (ft_strncmp(name, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(name, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(name, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(name, "export", 7) == 0)
		return (1);
	if (ft_strncmp(name, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(name, "env", 4) == 0)
		return (1);
	if (ft_strncmp(name, "exit", 5) == 0)
		return (1);
	return (0);
}

int	run_builtin(t_shell *sh, char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (bt_echo(argv));
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (bt_cd(sh, argv));
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (bt_pwd(sh));
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (bt_export(sh, argv));
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (bt_unset(sh, argv));
	if (ft_strncmp(argv[0], "env", 4) == 0)
		return (bt_env(sh));
	return (bt_exit(sh, argv));
}
