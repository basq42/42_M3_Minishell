/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:24:41 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/23 11:24:55 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Looks at line[i] which is know to be either | or < or >
 * and decides which opertor follows from here
 * IMPORTANT: 2 char operators are checked before as otherwise "<<"
 * would be read as "<" + "<"
 * Returns the operator typee
 * Called by: read_operator
 * */
static t_type	get_operator_type(char *line, int i)
{
	if (line[i] == '|')
		return (T_PIPE);
	if (line[i] == '<' && line[i + 1] == '<')
		return (T_HEREDOC);
	if (line[i] == '<')
		return (T_IN);
	if (line[i] == '>' && line[i + 1] == '>')
		return (T_APPEND);
	return (T_OUT);
}

/*
 * Constructs the operator that starts at line[*i], appends it to list
 * and moves *i past the operator.
 * STEPS:
 *	1. Identify the operator type.
 *	2. Create a token with value NULL (as tokens dont carry text)
 *	3. Append token to list
 *	4. Move *i forward by full operator.
 *	Returns: 0 on ok, 1 on out of memory
 *	Called by: lexer()
 * */
static int	read_operator(char *line, int *i, t_token **tokens)
{
	t_type	type;
	t_token	*token;

	type = get_operator_type(line, *i);
	token = token_new(type, NULL);
	if (token == NULL)
		return (1);
	token_add_back(tokens, token);
	if (type == T_HEREDOC || type == T_APPEND)
		*i = *i + 2;
	else
		*i = *i + 1;
	return (0);
}

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
		if (!quote && (line[*i] == '\'' || line[*i] == '"'))
			quote = line[*i];
		else if (line[*i] == quote)
			quote = 0;
		(*i)++;
	}
	return (quote);
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
	value = ft_substr(line, start, *i - start);
	if (!value)
		return (1);
	token = token_new(T_WORD, value);
	if (!token)
	{
		free(value);
		return (1);
	}
	token_add_back(tokens, token);
	return (0);
}

/*
 * Cuts the entire input line into a list of tokens, from left to right.
 * NO grammer check (parser does it) and NO expansion of $ or quote removal
 * (expandar does those). This only detects if there is an unclosed quote.
 *
 * Steps:
 *	1. Start with empty list and i = 0
 *	2. Loop untill the end of line reached, and at each position:
 *		2.1. skip " " or "\t" as they delimit the tokens.
 *		2.2. if: | or < or > read an operator token.
 *		2.3. else: read a WORD token.
 *	3. If fail on reading a token -> free all the tokens created till now and
 *	set the list to NULL and return error code.
 *
 *	A line with only SPACEs gives an empty list and returns 0.
 *	Returns: 0 on ok, 1 on out of memory, 2 on unclosed quote.
 *	Called by: run_line()
 * */
int	lexer(char *line, t_token **tokens)
{
	int	i;
	int	status;

	*tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		status = 0;
		if (is_space(line[i]) == 1)
			i += 1;
		else if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			status = read_operator(line, &i, tokens);
		else
			status = read_word(line, &i, tokens);
		if (status != 0)
		{
			free_tokens(*tokens);
			*tokens = NULL;
			return (status);
		}
	}
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
