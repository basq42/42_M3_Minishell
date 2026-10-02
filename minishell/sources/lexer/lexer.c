#include "minishell.h"



/*
 * Advances *i to the end of one word: stops at a metachar:
 * (space, tab, | < >) unless inside quotes. Quotes are kept as per bash.
 * Returns: 0 on all quotes closed, if not the unclosed quote char.
 * Called by: read_word
 * */
static char	scan_word(char *line, int *i)
{
	char	quote;

	quote = 0;
	while (line[*i] && (quote || !is_meta(line[*i])))
	{
		if (!quote && (line[*i] == '\' || line[*i] == '"'))
			quote = line[*i];
		else if (line[*i] == quote)
			quote = 0;
		(*i)++;
	}
}

/*
 * Cuts one word to TOKEN (raw text and quotes included) and appends it.
 * Returns: 0 on ok, 1 on out of memory, 2 on unclosed quote (error print).
 * Called by: lexer
 * */
static int	read_word(char *line, int *i, t_token **tokens)
{
	int		start;
	char	*value;
	t_token	*token;

	start = *i;
	if (scan_word(line, i) != 0)
	{
		print_error("syntax error: unclosed quote", NULL, NULL);
		return (2);
	}
	value = token_new(T_WORD, value);
	if (!token)
	{
		free(value);
		return (1);
	}
	token_add_back(tokens, token);
	return (0);
}
/* PSEUDOCODE

# Reads one operator (| < > << >>) and appends its token.
# Returns 0 ok, 1 oom. Called by: lexer.
read_operator(line, &i, &tokens) -> int:      # static, defined before lexer
	if line[i] == '|':                           type = T_PIPE,    len = 1
	else if line[i] == '<' and line[i+1] == '<': type = T_HEREDOC, len = 2
	else if line[i] == '<':                      type = T_IN,      len = 1
	else if line[i] == '>' and line[i+1] == '>': type = T_APPEND,  len = 2
	else:                                        type = T_OUT,     len = 1
	tok = token_new(type, NULL), if NULL return 1
	token_add_back(&tokens, tok)
	i += len
	return 0

# Splits a whole input line into tokens. On error the list is freed.
# Returns 0 ok, 1 oom, 2 syntax. Called by: run_line.
lexer(line, &tokens) -> int:
	i = 0
	while line[i]:
		if is_space(line[i]): i++, continue
		if line[i] is '|', '<' or '>': status = read_operator(line, &i, &tokens)
		else:                          status = read_word(line, &i, &tokens)
		if status != 0: free_tokens(tokens), tokens = NULL, return status
	return 0
*/
