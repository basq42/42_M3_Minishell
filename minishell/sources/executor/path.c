/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:21:44 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 12:34:43 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * ISSUE: minishell$> . | .. | .
 * 			minishell$> .: Is a directory
 * 			minishell$> ..: Is a directory
 *  While Bash does:
 *  		~$ . | .. | .
			bash: .: filename argument required
				.: usage: . filename [arguments]
			bash: .: filename argument required
				.: usage: . filename [arguments]
				..: command not found
	FIX: check whether file or path
 find_in_path takes command name (a WORD without /, like ls) and PATH variable
 and then splits PATH into directories. For each dir, search_directories builds
 candidate path and checks if it exists and returns first existing candidate as
 the command to run, something like:
 name = "ls"      dirs = /home/$USER/bin, /usr/local/bin, /usr/bin, ...
candidate 1: /home/$USER/bin/ls     access(F_OK) fails   -> next
candidate 2: /usr/local/bin/ls     access(F_OK) fails   -> next
candidate 3: /usr/bin/ls           access(F_OK) works   -> returns it
SO FIX: instead of checking if it exists, now checks if it is a regular file
via using the system-def. file and directory metedata struct. Then stat()
requests kernel to fill for path. S_ISREG = tru for file and =false for rest
 * */
static int	is_regular_file(char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

/* OLD:
 * Tries "dir/name" for each directory in directories, picks the first existing
 * file.
 * ----------------------------
 * NEW:
 * Picks the first regular file.
 * Returns: mallocd full path or NULL if not found.
 * If malloc failure then returns NULL and set *oom (out of memory) to 1.
 * Called by: find_in_path;
 */
static char	*search_directories(char **dirs, char *name, int *oom)
{
	char	*part;
	char	*full;
	int		i;

	i = 0;
	while (dirs[i])
	{
		part = ft_strjoin(dirs[i], "/");
		full = NULL;
		if (part)
			full = ft_strjoin(part, name);
		free(part);
		if (!full)
		{
			*oom = 1;
			return (NULL);
		}
		if (is_regular_file(full))
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

/*
 * Resolves a command name: (no '/') through shell's PATH variable.
 * Returns the mallocd full path. On fail, it prints the error and exits the
 * child (127 not found / PATH unset, 1 out of memory).
 * CHILD ONLY and called by exec_external.
 */
char	*find_in_path(t_shell *sh, char *name)
{
	char	*path_var;
	char	**dirs;
	char	*full;
	int		oom;

	path_var = env_get(sh->env, "PATH");
	if (!path_var)
		exec_fail(sh, name, "No such file or directory", 127);
	dirs = ft_split(path_var, ':');
	oom = (dirs == NULL);
	full = NULL;
	if (dirs)
		full = search_directories(dirs, name, &oom);
	free_strarr(dirs);
	if (oom)
		exec_fail(sh, "out of memory", NULL, 1);
	if (!full)
		exec_fail(sh, name, "command not found", 127);
	return (full);
}

/*

*Checks whether the PATH can be executed. If can't then prints the bash message.
 * Returns: 0 on ok, 127 on missing, 126 on directory or no exec permission.
 * Called by: exec_external;
 * int stat(const char *path, struct stat *buf);
 * Returns: 0 on success, -1 on fail and sets errno.
 */
int	check_exec_path(char *name, char *path)
{
	struct stat	st;

	if (stat(path, &st) == 1)
	{
		print_error(name, "No such file or directory", NULL);
		return (127);
	}
	if (S_ISDIR(st.st_mode))
	{
		print_error(name, "Is a directory", NULL);
		return (126);
	}
	if (access(path, X_OK) == -1)
	{
		print_error(name, "Permission denied", NULL);
		return (126);
	}
	return (0);
}
