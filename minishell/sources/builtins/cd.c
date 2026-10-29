/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:04:20 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 16:52:02 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	target_home(t_shell *sh, char **target)
{
	char	*home;

	home = env_get(sh->env, "HOME");
	if (!home)
	{
		print_error("cd", "HOME not set", NULL);
		return (1);
	}
	*target = ft_strdup(home);
	return (*target == NULL);
}

static int	target_oldpwd(t_shell *sh, char **target)
{
	char	*oldpwd;

	oldpwd = env_get(sh->env, "OLDPWD");
	if (!oldpwd)
	{
		print_error("cd", "OLDPWD not set", NULL);
		return (1);
	}
	*target = ft_strdup(oldpwd);
	return (*target == NULL);
}

/*
 * ISSUE on: cd ""
 * 	bash does: nothing happens as cd on empty string is no-op (so keeps $? = 0)
 * 	while minishell throws error and sets $? = 1
 * WHY: resolve_target() copies the arg as is, leading to chdir("") which gets
 * rejected by kernel with ENOENT so bt_cd() prints strerror(errno) and returns
 * 1.
 * FIX: map an empty argument to "." (current directory). chdir(".") always
 * succeeds so build_new_pwd(old_pwd, ".") skipts . so PWD stays the same.
 * */
static int	resolve_target(t_shell *sh, char **argv, char **target)
{
	if (argv[1] && argv[2])
	{
		print_error("cd", "too many arguments", NULL);
		return (1);
	}
	if (!argv[1])
		return (target_home(sh, target));
	if (ft_strncmp(argv[1], "-", 2) == 0)
		return (target_oldpwd(sh, target));
	if (argv[1][0] == '\0')
		*target = ft_strdup(".");
	else
		*target = ft_strdup(argv[1]);
	return (*target == NULL);
}

static void	apply_new_pwd(t_shell *sh, char *old_pwd, char *new_pwd,
		int is_dash)
{
	if (old_pwd)
		env_set(&sh->env, "OLDPWD", old_pwd);
	env_set(&sh->env, "PWD", new_pwd);
	if (is_dash)
	{
		ft_putstr_fd(new_pwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

int	bt_cd(t_shell *sh, char **argv)
{
	char	*target;
	char	*old_pwd;
	char	*new_pwd;
	int		is_dash;

	if (resolve_target(sh, argv, &target))
		return (1);
	is_dash = argv[1] && ft_strncmp(argv[1], "-", 2) == 0;
	if (chdir(target) == -1)
	{
		print_error("cd", target, strerror(errno));
		free(target);
		return (1);
	}
	old_pwd = env_get(sh->env, "PWD");
	new_pwd = build_new_pwd(old_pwd, target);
	free(target);
	if (!new_pwd)
	{
		print_error("out of memory", NULL, NULL);
		return (1);
	}
	apply_new_pwd(sh, old_pwd, new_pwd, is_dash);
	free(new_pwd);
	return (0);
}
