/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_strbuf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:07:01 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:07:22 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*char_to_str(char c)
{
	char	*s;

	s = malloc(2);
	if (!s)
		return (NULL);
	s[0] = c;
	s[1] = '\0';
	return (s);
}

char	*join_free(char *a, char *b)
{
	char	*joined;

	if (!a || !b)
	{
		free(a);
		free(b);
		return (NULL);
	}
	joined = ft_strjoin(a, b);
	free(a);
	free(b);
	return (joined);
}
