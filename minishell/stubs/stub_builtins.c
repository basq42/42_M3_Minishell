#include "minishell.h"

/* ==========================================================================
 * TEST STUB - NOT PART OF THE SUBMISSION - DELETE stubs/ BEFORE SUBMITTING
 * Stands in for Bahtiyor's builtins/ until the real one lands.
 * Built only by "make stubs" (binary: minishell_stubs), never by "make".
 * Simple versions of the 7 builtins, enough to test Baran's executor and
 * env/ code. Output only with write / ft_putstr_fd (never printf: a lone
 * builtin's stdout is restored with dup2 right after it returns).
 * Simplifications: exit takes the number as given (no overflow check),
 * export's "declare -x" listing is not sorted.
 * ========================================================================== */

static int	same(const char *a, const char *b)
{
	return (ft_strncmp(a, b, ft_strlen(b) + 1) == 0);
}

/* echo [-n...] [words]: -n, -nn, -nnn... at the start mean "no newline". */
static int	builtin_echo(char **argv)
{
	int	i;
	int	j;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] != NULL && argv[i][0] == '-' && argv[i][1] == 'n')
	{
		j = 1;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i][j] != '\0')
			break ;
		newline = 0;
		i++;
	}
	while (argv[i] != NULL)
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

/* pwd: the current directory, from getcwd. */
static int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		print_error("pwd", strerror(errno), NULL);
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	return (0);
}

/* cd [dir]: no argument means $HOME; updates OLDPWD and PWD. */
static int	builtin_cd(t_shell *sh, char **argv)
{
	char	*target;
	char	*old;
	char	*now;

	if (argv[1] != NULL && argv[2] != NULL)
	{
		print_error("cd", "too many arguments", NULL);
		return (1);
	}
	target = argv[1];
	if (target == NULL)
		target = env_get(sh->env, "HOME");
	if (target == NULL)
	{
		print_error("cd", "HOME not set", NULL);
		return (1);
	}
	old = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		free(old);
		print_error("cd", target, strerror(errno));
		return (1);
	}
	now = getcwd(NULL, 0);
	if (old != NULL)
		env_set(&sh->env, "OLDPWD", old);
	if (now != NULL)
		env_set(&sh->env, "PWD", now);
	free(old);
	free(now);
	return (0);
}

/* env: every variable that has a value, as KEY=value. */
static int	builtin_env(t_shell *sh)
{
	t_env	*node;

	node = sh->env;
	while (node != NULL)
	{
		if (node->value != NULL)
		{
			ft_putstr_fd(node->key, STDOUT_FILENO);
			write(STDOUT_FILENO, "=", 1);
			ft_putstr_fd(node->value, STDOUT_FILENO);
			write(STDOUT_FILENO, "\n", 1);
		}
		node = node->next;
	}
	return (0);
}

/* Valid name: starts with a letter or _, then letters, digits or _. */
static int	valid_name(const char *s, size_t len)
{
	size_t	i;

	if (len == 0 || !(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (i < len)
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/* bash: minishell: export: `1a': not a valid identifier */
static int	not_valid(const char *cmd, const char *arg)
{
	char	*quoted;
	char	*tmp;

	tmp = ft_strjoin("`", arg);
	quoted = NULL;
	if (tmp != NULL)
		quoted = ft_strjoin(tmp, "'");
	free(tmp);
	if (quoted != NULL)
		print_error(cmd, quoted, "not a valid identifier");
	free(quoted);
	return (1);
}

/* export with no argument: every variable, as declare -x. */
static void	export_list(t_shell *sh)
{
	t_env	*node;

	node = sh->env;
	while (node != NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(node->key, STDOUT_FILENO);
		if (node->value != NULL)
		{
			write(STDOUT_FILENO, "=\"", 2);
			ft_putstr_fd(node->value, STDOUT_FILENO);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		node = node->next;
	}
}

/* export KEY=value / export KEY: through env_set (Baran's env/). */
static int	builtin_export(t_shell *sh, char **argv)
{
	int		i;
	int		status;
	char	*equal;
	char	*key;

	if (argv[1] == NULL)
	{
		export_list(sh);
		return (0);
	}
	status = 0;
	i = 0;
	while (argv[++i] != NULL)
	{
		equal = ft_strchr(argv[i], '=');
		if (equal == NULL)
			equal = argv[i] + ft_strlen(argv[i]);
		if (!valid_name(argv[i], equal - argv[i]))
		{
			status = not_valid("export", argv[i]);
			continue ;
		}
		key = ft_substr(argv[i], 0, equal - argv[i]);
		if (key == NULL)
			return (1);
		if (*equal == '=')
			env_set(&sh->env, key, equal + 1);
		else
			env_set(&sh->env, key, NULL);
		free(key);
	}
	return (status);
}

/* unset KEY...: through env_unset (Baran's env/). */
static int	builtin_unset(t_shell *sh, char **argv)
{
	int	i;

	i = 1;
	while (argv[i] != NULL)
		env_unset(&sh->env, argv[i++]);
	return (0);
}

/* exit [n]: ends the shell through shell_exit (frees everything). */
static int	builtin_exit(t_shell *sh, char **argv)
{
	int	i;

	if (argv[1] == NULL)
		shell_exit(sh, sh->last_status);
	i = 0;
	if (argv[1][i] == '-' || argv[1][i] == '+')
		i++;
	if (argv[1][i] == '\0')
		i = -1;
	while (i >= 0 && argv[1][i] != '\0' && ft_isdigit(argv[1][i]))
		i++;
	if (i < 0 || argv[1][i] != '\0')
	{
		print_error("exit", argv[1], "numeric argument required");
		shell_exit(sh, 2);
	}
	if (argv[2] != NULL)
	{
		print_error("exit", "too many arguments", NULL);
		return (1);
	}
	shell_exit(sh, ft_atoi(argv[1]) & 255);
	return (0);
}

/* Contract: 1 for the 7 builtins of the subject, else 0. */
int	PLACE_HOLDER_IS_BUILTIN_FUNC(char *name)
{
	return (same(name, "echo") || same(name, "cd") || same(name, "pwd")
		|| same(name, "export") || same(name, "unset")
		|| same(name, "env") || same(name, "exit"));
}

/* Contract: runs the builtin, returns its exit status. */
int	PLACE_HOLDER_RUN_BUILTIN_FUNC(t_shell *sh, char **argv)
{
	if (same(argv[0], "echo"))
		return (builtin_echo(argv));
	if (same(argv[0], "pwd"))
		return (builtin_pwd());
	if (same(argv[0], "cd"))
		return (builtin_cd(sh, argv));
	if (same(argv[0], "env"))
		return (builtin_env(sh));
	if (same(argv[0], "export"))
		return (builtin_export(sh, argv));
	if (same(argv[0], "unset"))
		return (builtin_unset(sh, argv));
	return (builtin_exit(sh, argv));
}
