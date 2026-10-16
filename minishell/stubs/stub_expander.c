#include "minishell.h"

/* ==========================================================================
 * TEST STUB - NOT PART OF THE SUBMISSION - DELETE stubs/ BEFORE SUBMITTING
 * Stands in for Bahtiyor's expander/ until the real one lands.
 * Built only by "make stubs" (binary: minishell_stubs), never by "make".
 * Follows the contract in the implementation guide, Section 12:
 *	- $VAR and $? are expanded, except inside '...';
 *	- quotes are removed from argv words and redirection file names;
 *	- an UNQUOTED word that expands to "" is removed from argv;
 *	- heredoc delimiters (T_HEREDOC) are left raw;
 *	- returns 0 ok, 1 out of memory (and nothing else).
 * Not done (not needed for the mandatory tests): word splitting of
 * unquoted expansions ("$A" with spaces stays one word).
 * ========================================================================== */

/* Appends the first len chars of s to *result. 0 ok, 1 out of memory. */
static int	append(char **result, const char *s, size_t len)
{
	char	*part;
	char	*joined;

	if (*result == NULL)
		return (1);
	part = ft_substr(s, 0, len);
	if (part == NULL)
		return (1);
	joined = ft_strjoin(*result, part);
	free(part);
	free(*result);
	*result = joined;
	return (joined == NULL);
}

/*
 * s points at a '$'. Appends what it expands to and returns how many
 * characters of s were used:
 *	$?     -> the last status            (2 chars)
 *	$NAME  -> its value, or nothing      (1 + length of NAME)
 *	$ alone or $ + other char -> "$"      (1 char)
 */
static size_t	expand_dollar(t_shell *sh, const char *s, char **result)
{
	size_t	len;
	char	*name;
	char	*value;

	if (s[1] == '?')
	{
		value = ft_itoa(sh->last_status);
		if (value == NULL || append(result, value, ft_strlen(value)))
		{
			free(value);
			free(*result);
			*result = NULL;
			return (2);
		}
		free(value);
		return (2);
	}
	if (!(ft_isalpha(s[1]) || s[1] == '_'))
	{
		append(result, "$", 1);
		return (1);
	}
	len = 1;
	while (ft_isalnum(s[len]) || s[len] == '_')
		len++;
	name = ft_substr(s, 1, len - 1);
	value = NULL;
	if (name != NULL)
		value = env_get(sh->env, name);
	free(name);
	if (value != NULL)
		append(result, value, ft_strlen(value));
	return (len);
}

/*
 * Builds a new string from s.
 *	do_quotes: ' and " open and close quotes and are removed.
 *	do_dollar: $ is expanded, except inside '...'.
 * Returns the new string, or NULL on out of memory.
 */
static char	*expand_word(t_shell *sh, const char *s, int do_quotes, int do_dollar)
{
	char	*result;
	char	quote;
	size_t	i;

	result = ft_strdup("");
	quote = 0;
	i = 0;
	while (result != NULL && s[i] != '\0')
	{
		if (do_quotes && quote == 0 && (s[i] == '\'' || s[i] == '"'))
			quote = s[i++];
		else if (do_quotes && quote != 0 && s[i] == quote)
		{
			quote = 0;
			i++;
		}
		else if (do_dollar && s[i] == '$' && quote != '\'')
			i += expand_dollar(sh, s + i, &result);
		else
		{
			append(&result, s + i, 1);
			i++;
		}
	}
	return (result);
}

/* Expands every word of one argv, dropping unquoted words that became "". */
static int	expand_argv(t_shell *sh, t_cmd *cmd)
{
	char	**new_argv;
	char	*word;
	int		count;
	int		i;
	int		j;

	count = 0;
	while (cmd->argv[count] != NULL)
		count++;
	new_argv = malloc((count + 1) * sizeof(char *));
	if (new_argv == NULL)
		return (1);
	i = 0;
	j = 0;
	while (i < count)
	{
		word = expand_word(sh, cmd->argv[i], 1, 1);
		if (word == NULL)
		{
			new_argv[j] = NULL;
			free_strarr(new_argv);
			return (1);
		}
		if (word[0] == '\0' && !ft_strchr(cmd->argv[i], '\'')
			&& !ft_strchr(cmd->argv[i], '"'))
			free(word);
		else
			new_argv[j++] = word;
		i++;
	}
	new_argv[j] = NULL;
	free_strarr(cmd->argv);
	cmd->argv = new_argv;
	return (0);
}

/* Contract: expand every argv word and every non-heredoc file name. */
int	PLACE_HOLDER_EXPAND_CMDS_FUNC(t_shell *sh)
{
	t_cmd	*cmd;
	t_redir	*redir;
	char	*file;

	cmd = sh->cmds;
	while (cmd != NULL)
	{
		if (expand_argv(sh, cmd) != 0)
			return (1);
		redir = cmd->redirs;
		while (redir != NULL)
		{
			if (redir->type != T_HEREDOC)
			{
				file = expand_word(sh, redir->file, 1, 1);
				if (file == NULL)
					return (1);
				free(redir->file);
				redir->file = file;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

/* Contract (heredoc body): expand $VAR and $?, keep every quote. */
char	*PLACE_HOLDER_EXPAND_STRING_FUNC(t_shell *sh, char *s)
{
	return (expand_word(sh, s, 0, 1));
}

/* Contract (heredoc delimiter): remove quotes, expand nothing. */
char	*PLACE_HOLDER_REMOVE_QUOTES_FUNC(char *s)
{
	return (expand_word(NULL, s, 1, 0));
}
