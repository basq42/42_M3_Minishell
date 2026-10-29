/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:04:36 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:04:49 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_digits(char *arg, int i, long *n)
{
	*n = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		*n = *n * 10 + (arg[i] - '0');
		i++;
	}
	return (1);
}

static int	parse_status(char *arg, int *ok)
{
	long	n;
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	*ok = arg[i] && parse_digits(arg, i, &n);
	if (!*ok)
		return (0);
	n = n * sign;
	return (((n % 256) + 256) % 256);
}

int	bt_exit(t_shell *sh, char **argv)
{
	int	ok;
	int	status;

	if (!argv[1])
		shell_exit(sh, sh->last_status);
	status = parse_status(argv[1], &ok);
	if (!ok)
	{
		print_error("exit", argv[1], "numeric argument required");
		shell_exit(sh, 2);
	}
	if (argv[2])
	{
		print_error("exit", "too many arguments", NULL);
		return (1);
	}
	shell_exit(sh, status);
	return (0);
}
