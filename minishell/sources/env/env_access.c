/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:18:15 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:18:39 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Finds the node whose key is EXACTLY key.
 * Calls ft_strncmp with ft_strlen(key)+1 to check '\0' as well.
 * Looking for "PATH" should not match "PATHX" or "PA".
 * Returns: the BORROWED node on ok, NULL if not found.
 * Caller: env_get, env_set.
 * */
static t_env	*env_find(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

/*
 * Read the value of a variable. Example:
 * 	env_get(sh->env, "HOME") => "/home/user"
 * Returns: a BORROWED pointer to the list, or NULL if variable doesnt exist or
 * has no value (such as in the case: export X)
 * Caller: find_in_path; BUT EXPANDER AND SOME BUILTIN COMMANDS WILL NEED TO
 * CALL IT TOO!:
 * 		- if expander reads $VAR with env_get: a NULL result MUST EXPAND to an
 * 		empty string.
 * 		- if CD reads HOME or OLDPWD with env_get: NULL means 'not set' in which
 * 		case og bash prints: "cd: HOME not set";
 * 		- if they read the list some other way, then only find_in_path will call
 * 		this.
 * */
char	*env_get(t_env *env, char *key)
{
	t_env	*node;

	node = env_find(env, key);
	if (!node)
		return (NULL);
	return (node->value);
}

/* MALLOC HERE!
 * Add a new variable at the end of list.
 * */
static int	env_add_new(t_env **env, char *key, char *value_copy)
{
	char	*key_copy;
	t_env	*node;

	key_copy = ft_strdup(key);
	node = NULL;
	if (key_copy)
		node = env_new(key_copy, value_copy);
	if (!node)
	{
		free(key_copy);
		free(value_copy);
		return (1);
	}
	env_add_back(env, node);
	return (0);
}

/*
 * Creates or edits a variable if (as per bash):
 * 		- key exists, value given	=> old value replaced
 * 		- key missing				=> a new variable is added at end.
 * 		- value == NULL:
 * 				- key exists		=> no change
 * 				- key missing		=> X is declared without a value.
 * 	Both string are copied: the caller's strings usually belong to argv, which
 * 	is freed right after command. The list owns all its MEMORY.
 * 	Returns: 0 on ok, 1 on oom (with list left unchanged)
 *
 * 	CALLER WILL BE THE BUILTINS! HENCE DEPENDABLE ON:
 * 		- if EXPORT will use env_set then call with parameters:
 * 			"export KEY=value"	-> env_set(&sh->env, "KEY", "value");
 * 			"export KEY"		-> env_set(&sh->env, "KEY, NULL");
 * 		- if CD will use env_set: it should use to update PWD and OLDPWD after
 * 		changing directory.
 * 		- IF YOU DECIDE NOT TO USE AT ALL, then tell me and I will delete this
 * 		function!
 * */
int	env_set(t_env **env, char *key, char *value)
{
	t_env	*node;
	char	*value_copy;

	dbg_str("env", "set", key);
	node = env_find(*env, key);
	if (node && !value)
		return (0);
	value_copy = NULL;
	if (value)
	{
		value_copy = ft_strdup(value);
		if (!value_copy)
			return (1);
	}
	if (node)
	{
		free(node->value);
		node->value = value_copy;
		return (0);
	}
	return (env_add_new(env, key, value_copy));
}

/*
 * Removes a variable which is "unset KEY". Does nothing if the key does not
 * exists as per bash: unset of an unknown name is not an error.
 * It is a SIGNLY LINKED LIST so to remove a node, the node before it must point
 * to past it!:
 * 		prev -> cur -> next		becomes		prev -> next;
 * If current node is the first node then there is no prev node, so the head
 * itself moves on whcih is why env is the ADDRESS OF THE HEAD POINTER.
 *
 * CALLER's ARE THE BUILTINS:
 * 	- if UNSET will use env_unset: one call per name
 * 		"unset A B"	-> env_unset(&sh->env, "A") + env_unset(&sh->env, "B");
 * 	- if UNSET removes the NODE itself: then tell me and I will delete this
 * 	function!
 * */
void	env_unset(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*cur;

	prev = NULL;
	cur = *env;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev == NULL)
				*env = cur->next;
			else
				prev->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
