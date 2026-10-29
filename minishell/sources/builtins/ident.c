/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:03:55 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 15:29:20 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ISSUE: error message differs from bash's.
 * On invalid name bash does:
 * 	bash$ export 1A=2
 *	bash$ export: `1A=2': not a valid identifier
 *	bash$ echo $?
 *	bash$ 1
 *
 * Minishell, does not print the ` instead it does:
 * 	minishell: export: 1A=2: not a valid identifier.
 * WHY? print_error is not made to add the `' around quotes.
 * FIX: ft_putstr_fd calls to format message as bash's
 * */
void	print_ident_error(char *cmd, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/* valid shell names: [A-Za-z_][A-Za-z0-9_]
 * */
int	is_valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
