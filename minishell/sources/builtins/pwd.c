/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:04:29 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 17:00:27 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * bash's pwd prints $PWD ONLY IF it names the current directory. Otherwise, it
 * asks the KERNEL for the REAL PATH.
 * ISSUE:
 * 	bash$ export PWD=/foo; pwd;
 * 	bash$ [real current directory]
 * 	minishell$ export PWD=/foo; pwd;
 * 	minishell$ /foo
 * 	FIX: use stat to check if the name is valid.
 *		stat returns a file's device number (st_dev) and i-node number (st_ino).
 *		Together they identify a directory, regardless of the path text. If $PWD
 *		and . results the same pair, $PWD is the current direcory, so its
 *		printed wich keeps the logical path (symlink names) maintaned by cd, as
 *		per bash. If they differ, or $PWD doesnt exist (stat fails), $PWD is
 *		stale so the code falls back to getcwd() to ask kernel the real path.
 * */
static int	is_current_dir(char *path)
{
	struct stat	a;
	struct stat	b;

	if (stat(path, &a) == -1 || stat(".", &b) == -1)
		return (0);
	return (a.st_dev == b.st_dev && a.st_ino == b.st_ino);
}

int	bt_pwd(t_shell *sh)
{
	char	*cwd;
	char	*pwd;

	pwd = env_get(sh->env, "PWD");
	if (pwd && is_current_dir(pwd))
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
