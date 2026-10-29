/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:04:29 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:06:30 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bt_pwd(t_shell *sh)
{
	char	*cwd;
	char	*pwd;

	pwd = env_get(sh->env, "PWD");
	if (pwd)
	{
		ft_putstr_fd(pwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_error("pwd", strerror(errno), NULL);
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	env_set(&sh->env, "PWD", cwd);
	free(cwd);
	return (0);
}
