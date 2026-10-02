#include "minishell.h"

/*
** is_space
** --------
** Tells whether c is a blank that separates words.
** Only space and tab count: those are the only blanks a readline line
** can contain (readline removes the final newline).
**
** Returns: 1 if c is a space or a tab, 0 otherwise.
** Called by: lexer, is_meta.
*/

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

/*
** is_meta
** -------
** Tells whether c is a metacharacter: a character that ENDS a word
** when it appears outside quotes.
** The subject only requires | < > as operators, plus the blanks.
** Backslash and semicolon are NOT here on purpose: the subject says not
** to interpret them, so they are ordinary characters inside a word.
**
** Returns: 1 if c is a metacharacter, 0 otherwise.
** Called by: scan_word.
*/
int	is_meta(char c)
{
	return (is_space(c) || c == '|' || c == '<' || c == '>');
}
