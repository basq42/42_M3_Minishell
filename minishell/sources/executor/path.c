#include "minishell.h"

/*
 * Tries "dir/name" for each directory in directories, picks the first existing
 * file.
 * Returns: mallocd full path or NULL if not found.
 * If malloc failure then returns NULL and set *oom (out of memory) to 1.
 * Called by: find_in_path;
 */
static char	*search_directories(char **dirs, char *name, int *oom)
{
	char	*part;
	char	*full;
	int		i;

	i = -1;
	while (dirs[i++])
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
		if (access(full, F_OK) == 0)
			return (full);
		free(full);
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

	* Checks whether the PATH can be executed. If can't then prints the bash message.
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
