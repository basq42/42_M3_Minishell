/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:04:36 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:57:12 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ISSUE: overflow not checked:
 * 	bash$ exit 9223372036854775808
 * 	bash$ numeric argument required
 * 	bash$ echo $?
 * 	bash$ 2
 * Ours:
 * 	minishell$ exit 9223372036854775808
 * 	exits with status = 0
 *
 * FIX: limit check: *n > (limit - (argv[i] - '0')) / 10 before multiplication.
 * 		+ using unsigned long instead of long:
 * 			Signed long overflow is undefined behaviour while unsigned long
 * 			is wraped usign %2^64 and defined. + |LONG_MIN| does not fit long
 * */
static int	parse_digits(char *arg, int i, unsigned long limit,
		unsigned long *n)
{
	*n = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		if (*n > ((limit - (arg[i] - '0')) / 10))
			return (0);
		*n = *n * 10 + (arg[i] - '0');
		i++;
	}
	return (1);
}

/*
 * limit = LONG_MAX for postive, LONG_MAX + 1 for negative (|long_min| =
 * long_max + 1).
 * The negative case is done on the unsigned value (256 - n % 256) to avoid
 * conversion of LONG_MIN to a signed type.
 * */
static int	parse_status(char *arg, int *ok)
{
	unsigned long	n;
	unsigned long	limit;
	int				i;
	int				sign;

	i = 0;
	sign = 1;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	limit = LONG_MAX;
	if (sign == -1)
		limit = (unsigned long)LONG_MAX + 1;
	*ok = arg[i] && parse_digits(arg, i, limit, &n);
	if (!*ok)
		return (0);
	if (sign == -1)
		return ((256 - n % 256) % 256);
	return (n % 256);
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
