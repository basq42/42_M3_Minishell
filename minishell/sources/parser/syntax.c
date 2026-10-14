#include "minishell.h"

/*
MALLOC (in ft_strjoin)!!!
	On syntax error,
		bash prints the error message for the token where the grammer failed,
		and gives exit
	status 2. This function copies that.

 if token == NULL then the line ended where something was still expected,
	for which bash calls 'newline':
	"ls |"          -> minishell: syntax error near unexpected token `newline'
	"ls | | cat"    -> minishell: syntax error near unexpected token `|'
	"ls > >"        -> minishell: syntax error near unexpected token `>'
Dont MIX: '' with ``

token_text() gives the text for operator errors as operator tokens have value == NULL
As a WORD is never unexpected -> only operators reach this function!
If malloc fails -> message is shorter but status is kept at 2.
Returns: always 2
Caller: check_syntax()
*/
int	syntax_error(t_token *token)
{
	const char	*text;
	char		*start;
	char		*msg;

	text = "newline";
	if (!token)
		text = token_text(token->type);
	start = ft_strjoin("syntax error near unexpected token `", text);
	msg = NULL;
	if (!start)
		msg = ft_strjoin(start, "'");
	free(start);
	if (!msg)
		print_error("syntax error", NULL, NULL);
	else
		print_error(msg, NULL, NULL);
	free(msg);
	return (2);
}

/*

	* PASS 1 of the parser (validate then parse): checks if the token list is a valid line: local grammer
 * The check depends on 3 RULES:
 * 		1. THE FIRST TOKEN: is it an token allowed to start a line?
 * 		2. THE LAST TOKEN: is it a token allowed to end a line?
 * 		3. NEIGHBOURING PAIRS: is each pair an allowed pair?
 * 	In this case this mandates: abbrv: W = WORD, P = Pipe, R =
 * 	redirection
 * 		1. FIRST TOKEN: 		Either W or R, and NOT P
 * 		2. LAST TOKEN: 			W, and neither R nor P
 * 		3. NEIGHBOURING PAIRS:
 * 				3.1: Total set is 3*3=9
 * 				3.2: Valid Pairs are:
 * 										WW (ls -l)
 * 										WP (ls | wc)
 * 										WR (ls > f)
 * 										PW (ls | wc)
 * 										PR (ls | > f)
 * 										RW (> f)
 *  Hence any combination beyond the above is invalid, and the first instance of
 *  it is reported (as per bash). The code is a negative of the ruleset.
 *  Returns: 0 on valid line, 2 on the first instance of syntax error.
 *  Caller: parser().
 * */
int	check_syntax(t_token *tokens)
{
	t_token	*cursor;
	t_token	*next;

	if (tokens && tokens->type == T_PIPE)
		return (syntax_error(tokens));
	cursor = tokens;
	while (cursor)
	{
		next = cursor->next;
		if (cursor->type == T_PIPE && (next == NULL || next->type == T_PIPE))
			return (syntax_error(next));
		if (cursor->type != T_PIPE && cursor->type != T_WORD && (next == NULL
				|| next->type != T_WORD))
			return (syntax_error(next));
		cursor = next;
	}
	return (0);
}
