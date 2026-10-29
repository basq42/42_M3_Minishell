/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:07:16 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:07:23 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *s)
{
	char	*out;
	int		i;
	int		sq;
	int		dq;

	out = ft_strdup("");
	i = 0;
	sq = 0;
	dq = 0;
	while (s[i] && out)
	{
		if (!dq && s[i] == '\'')
			sq = !sq;
		else if (!sq && s[i] == '"')
			dq = !dq;
		else
			out = join_free(out, char_to_str(s[i]));
		i++;
	}
	return (out);
}

char	*expand_string(t_shell *sh, char *s)
{
	char	*out;
	int		i;

	out = ft_strdup("");
	i = 0;
	while (s[i] && out)
	{
		if (s[i] == '$')
			out = join_free(out, read_var(sh, s, &i));
		else
			out = join_free(out, char_to_str(s[i++]));
	}
	return (out);
}
