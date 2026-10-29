/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:07:20 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 16:44:35 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_wscan(t_wscan *st)
{
	st->buf = ft_strdup("");
	st->touched = 0;
	st->sq = 0;
	st->dq = 0;
	st->fields = malloc(sizeof(char *));
	if (!st->buf || !st->fields)
	{
		free(st->buf);
		free(st->fields);
		return (1);
	}
	st->fields[0] = NULL;
	return (0);
}

/*
 * ISSUE: In bash $"..." is a locale translation syntax, while $'...' is an
 * escape-sequence syntax. When either appears outside double quotes, bash
 * drops the $ and reads the quoted part as is. Inside quotes, "$" is a literal
 * $
 * 		INPUT			BASH PRINT			minishell print (old)
 * 		echo $"USER"		USER				$USER
 * 		echo $'abc'			abc					$abc
 * 		echo "$"			$					$
 * 		echo "a$"			a$					a$
 * 	Why the DIFF: scan_dollar hands all $ to read_var. When the next char is a
 * 	quote (neither a letter, nor _), read_var takes the 'else' branch and
 * 	returns the literal "$" to then process the quote normally, leading to the
 * 	output starting with $ followed by quoted text.
 * 	FIX: check if:
 * 		$ is outside double quotes and directly before a quote	->skip $ and
 * 		emit nothing.
 * */
static int	scan_dollar(t_shell *sh, char *raw, int *i, t_wscan *st)
{
	char	*sub;
	int		j;
	int		status;

	if (!st->dq && (raw[*i + 1] == '"' || raw[*i + 1] == '\''))
	{
		(*i)++;
		return (0);
	}
	sub = read_var(sh, raw, i);
	if (!sub)
		return (1);
	j = 0;
	status = 0;
	while (sub[j] && status == 0)
		status = field_emit(st, sub[j++], st->dq);
	free(sub);
	return (status);
}

static int	scan_step(t_shell *sh, char *raw, int *i, t_wscan *st)
{
	char	c;

	c = raw[*i];
	if (!st->dq && c == '\'')
	{
		st->sq = !st->sq;
		st->touched = 1;
		(*i)++;
		return (0);
	}
	if (!st->sq && c == '"')
	{
		st->dq = !st->dq;
		st->touched = 1;
		(*i)++;
		return (0);
	}
	if (!st->sq && c == '$')
		return (scan_dollar(sh, raw, i, st));
	(*i)++;
	return (field_emit(st, c, st->sq || st->dq));
}

char	**expand_word(t_shell *sh, char *raw)
{
	t_wscan	st;
	int		i;
	int		status;

	if (init_wscan(&st))
		return (NULL);
	i = 0;
	status = 0;
	while (raw[i] && status == 0)
		status = scan_step(sh, raw, &i, &st);
	if (status == 0)
		status = field_close(&st);
	free(st.buf);
	if (status != 0)
	{
		free_strarr(st.fields);
		return (NULL);
	}
	return (st.fields);
}
