/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:18:31 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:19:26 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	THE SHELL's ENVIRONMENT
 *
 * The shell keeps its OWN COPY of the environment as a linked list of t_env
 * nodes, one per variable: { key "HOME", value "/home/user" }
 * List is built from main's envp with call to shell_init. So far the code reads
 * it for PATH lookup and to build the execve's environment.
 *
 * DEPENDS on expander/ and builtins/:
 * 	- the expander, export, unset, env, and cd are EXPECTED TO USE THIS SAME
 * 	LIST, through env_get, env_set, env_unset (refer to their comments).
 * 	If any of them use GETENV INSTEAD it would MISS EVERY CHANGE made in
 * 	minishell.
 * 	Why not getenv: getenv reads the environment the process started with. After
 * 	for example, "export X=1" or "unset PATH" typed in minishell, the original
 * 	environment has not changes sp getenv would give OLD VALUES.
 *
 * 	Why linked list: export adds variables and unset removes them and a lost
 * 	does both without moving or reallocating anything. the list is turned into a
 * 	'char **' by env_to_array only RIGHT BEFORE EXECVE, which requires that
 * 	format.
 *

 * 	value = NULL means "declared without a value"(such as in case of "export X")
 * 	This variable exists for EXPORT but env DOES NOT PRINT it and it is NOT
 * 	PASSED TO PROGRAMS as per bash (refer to env_to_array).
 * */
/*
 * MALLOC HERE!
 * allocates one node and stores the key and value in it. The node takes
 * OWNERSHIP of both: from now on they are FREED WITHIN THE NODE
 * (env_free or env_unset) so the CALLER MUST NOT FREE THEM (unless return case
 * 2)!
 * REturns: the node on ok, or NULL on oom (then the key and value are NOT FREED
 * SO THE CALLER STILL OWNS THEM!!)
 * Caller: node_from_entry, env_set through env_add_new
 * */
t_env	*env_new(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

/*
 * Appends a node at the end of the list SO variables keep the order of envp so
 * env prints them just as bash does.
 * Takes the ADDRESS of the HEAD pointer as an empty list's head must change.
 * Caller: env_init and env_add_new
 * */
void	env_add_back(t_env **list, t_env *node)
{
	t_env	*last;

	if (!*list)
	{
		*list = node;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = node;
}

/*
 * Frees the whole list. Saves NEXT before freeing the node to avoid reading
 * node->next after free(node).
 * Accepts NULL (an empty env)
 * Caller: ev_init (on failure), shell_exit
 * */
void	env_free(t_env *list)
{
	t_env	*next;

	while (list)
	{
		next = list->next;
		free(list->key);
		free(list->value);
		free(list);
		list = next;
	}
}

/*
 * Turns one envp entry of "KEY=value" into a node.
 * Splits at first '=' only: "A=b=c" will yield: key "A", value "b=c".
 * The Caller (env_init) already checked that entry contains '='
 * Returns: the node on ok, NULL on oom
 * */
static t_env	*node_from_entry(char *entry)
{
	char	*equal_sign;
	char	*key;
	char	*value;
	t_env	*node;

	equal_sign = ft_strchr(entry, '=');
	key = ft_substr(entry, 0, equal_sign - entry);
	value = ft_strdup(equal_sign + 1);
	node = NULL;
	if (key && value)
		node = env_new(key, value);
	if (!node)
	{
		free(key);
		free(value);
	}
	return (node);
}

/*
 * Builds the shells environment list from main's envp, once at STARTUP.
 * Entries without '=' are skipped as they are not a valid "KEY=value" pair.
 * envp may be empty (such as in case "env -i ./shell"). If so, the result is
 * then an empty list (NULL) which is NOT AN ERROR (shell_init is implemented
 * accordingly, refer its comments)
 * Returns: the list on ok, NULL if envp is empty or oom (in that case
 * semi-built list is freed first)
 * CALLER: shell_init.
 * */
t_env	*env_init(char **envp)
{
	t_env	*list;
	t_env	*node;
	int		i;

	list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			node = node_from_entry(envp[i]);
			if (!node)
			{
				env_free(list);
				return (NULL);
			}
			env_add_back(&list, node);
		}
		i++;
	}
	return (list);
}
