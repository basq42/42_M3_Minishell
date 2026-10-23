/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:18:54 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:18:55 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Counts the variables with value != NULL. WHY, refer comment on top of
 * env_to_array().
 * */
static int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}

/* MALLOC HERE!!
 * Build one "KEY=value" string from a node.
 * 2 ft_strjoin calls: "KEY" + "=" first, then + "value" with the middle string
 * freed immediatly.
 * Returns: new string on ok, NULL on oom.
 * */
static char	*make_entry(t_env *node)
{
	char	*key_equal;
	char	*entry;

	key_equal = ft_strjoin(node->key, "=");
	if (!key_equal)
		return (NULL);
	entry = ft_strjoin(key_equal, node->value);
	free(key_equal);
	return (entry);
}

/*
 * Bash treats exports:
 * 		$ export A=hello; export B=; export C
 * 		$ env
 * 		B=
 * 		A=hello
 * 		$ export
 *		.
 *		.
 *		.
 *		declare -x A="hello"
 *		declare -x B=""
 *		declare -x C
 *==============================================
 * Turning the list into a "KEY=value" array to pass onto execve as its env:
 * 		Shell Input			t_env Node
 * 		1. export A=hello	-> key "A", value "hello"
 * 		2. export B=		-> key "B", value ""
 * 		3. export C			-> key "C", value NULL
 * 	So the array will look as, for example:
 * 		{ "HOME=/home/user", "PATH=/usr/bin:/bin", ..., NULL }
 * 	Variables with value = NULL are skipped as per bash.
 * 	This array gets builts each time before every execve, so it always matches
 * 	the list after any export or unset.
 * 	If out of memory: array[i] is NULL at the failed slot, so free_strarr frees
 * 	exactly the entries made until this point, then the array itself.
 * 	Returns: the array on ok, NULL on oom.
 * 	Caller: exec_external()
 * 	MALLOC HERE!!!!
 * */
char	**env_to_array(t_env *env)
{
	char	**array;
	int		i;

	array = malloc((env_count(env) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			array[i] = make_entry(env);
			if (!array[i])
			{
				free_strarr(array);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
