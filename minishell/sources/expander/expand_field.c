/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_field.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/29 11:07:13 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:07:39 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	push_field(t_wscan *st, char *field)
{
	char	**new_fields;
	int		count;

	count = 0;
	while (st->fields[count])
		count++;
	new_fields = malloc((count + 2) * sizeof(char *));
	if (!new_fields)
	{
		free(field);
		return (1);
	}
	new_fields[count] = field;
	new_fields[count + 1] = NULL;
	while (count > 0)
	{
		count--;
		new_fields[count] = st->fields[count];
	}
	free(st->fields);
	st->fields = new_fields;
	return (0);
}

int	field_close(t_wscan *st)
{
	char	*old;
	int		status;

	old = st->buf;
	st->buf = ft_strdup("");
	status = 0;
	if (old[0] || st->touched)
		status = push_field(st, old);
	else
		free(old);
	st->touched = 0;
	if (!st->buf)
		return (1);
	return (status);
}

int	field_emit(t_wscan *st, char c, int quoted)
{
	char	*grown;

	if (!quoted && is_space(c))
		return (field_close(st));
	grown = join_free(st->buf, char_to_str(c));
	if (!grown)
		return (1);
	st->buf = grown;
	return (0);
}
