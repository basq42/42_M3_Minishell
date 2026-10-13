#include "minishell.h"

/*
 * MALLOC HERE: Allocate 1 token and fill.
 * Ownership: the token takes takes value, thus from here value is freed
 * together with the token (by free_tokens) so the caller must not free it
 * again!
 * Steps: malloc token -> Copy type and value into it -> set next to NULL
 * as new token is not yet linked.
 * Called by: read_operator, reead_word
 * */
t_token	*token_new(t_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

/*
 * Appends token to the end of the list to keep the tokens in the same order as
 * they appear in the line.
 * list is a pointer to the list's head poiter (t_token **), as when the list
 * is empty, the head needs to change.
 * Steps:
 * 1. Check if the list is empty --no--> token becomes head.
 *		--yes--> 2. Walk from head until the last node. --> 3. Link token
 * Called by: read_operator and read_word
 * */
void	token_add_back(t_token **list, t_token *token)
{
	t_token	*last;

	if (*list == NULL)
	{
		*list = token;
		return ;
	}
	last = *list;
	while (last->next != NULL)
		last->next = next;
	last->next = token;
}

/*
 * Lexer is designed to store operators without their text (a > becomes a token
 * of type T_OUT). Yet, on syntax error, the err message needs to print the
 * bad token for the user.
 * When syntax_check() find bad/unexpected token, it only has
 * next->type == T_PIPE with
 * value == NULL. So this function provides the text of the operator type.
 * return("") is a safe default for type == T_WORD, safe as syntax_check() never
 * calls token_text() on a type WORD
 * Called by: syntax_check()
 * */
const char	*token_text(t_type type)
{
	if (type == T_PIPE)
		return ("|");
	if (type == T_IN)
		return ("<");
	if (type == T_OUT)
		return (">");
	if (type == T_HEREDOC)
		return ("<<");
	if (type == T_APPEND)
		return (">>");
	return ("");
}
