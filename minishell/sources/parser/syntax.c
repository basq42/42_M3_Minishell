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
Caller: parse_command, parse_redirection
*/
int	syntax_error(t_token *token)
{
	const char	*text;
	char		*start;
	char		*msg;

	test = "newline";
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
