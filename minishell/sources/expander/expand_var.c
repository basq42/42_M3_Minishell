/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:07:05 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 15:55:42 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	var_name_len(char *raw, int start)
{
	int	len;

	len = 0;
	while (ft_isalnum(raw[start + len]) || raw[start + len] == '_')
		len++;
	return (len);
}

static char	*lookup_var(t_shell *sh, char *raw, int *i)
{
	int		start;
	int		len;
	char	*key;
	char	*value;

	start = *i + 1;
	len = var_name_len(raw, start);
	key = ft_substr(raw, start, len);
	*i = start + len;
	if (!key)
		return (NULL);
	value = env_get(sh->env, key);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

/* ISSUE:
minishell$ echo $1abc
$1abc
minishell$ echo $9
$9
minishell$ echo "$1abc"
$1abc
minishell$ echo $12
$12
minishell$
exit

While BASH Does:
abc instead of $1abc
empty line instead of $9
abc instead of $1abc
2 instead of $12

Why this issue: read_var has 3 branches:
	- $? gives the status;
	- $ followed by a letter or _ gives a variable lookup;
	- anything else gives a literal "$"
FIX: - treat a digit after $ as positional-parameter name
		- skip 2 chars to get 2 out of $12
expand_string also calls this.
 * */
char	*read_var(t_shell *sh, char *raw, int *i)
{
	if (raw[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(sh->last_status));
	}
	if (ft_isdigit(raw[*i + 1]))
	{
		*i += 2;
		return (ft_strdup(""));
	}
	if (!ft_isalpha(raw[*i + 1]) && raw[*i + 1] != '_')
	{
		*i += 1;
		return (ft_strdup("$"));
	}
	return (lookup_var(sh, raw, i));
}
