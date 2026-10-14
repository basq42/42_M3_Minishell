#include "minishell.h"

/*	MECHANISM (2 passes): VALIDATE (syntax_check) -> BUILD (here)
 *
 *	Here the lexer's token list is turned into the command list (t_cmd) with one
 *	t_cmd per command of the PIPELINE. Ex: "cat < input | wc -l"
 *					tokens: [cat] [T_IN] [input] [T_PIPE] [wc] [-l]
 *					cmd1: argv [cat],		redirs: [< input]
 *					cmd2: argv [wc, -l], redirs: none

		*	Only possible error path is Out of Memory (oom) as parser assumes valid input.
 *	This way, syntax error cannot be happen after an allocation, hence simpler.
 * */
/*
 * JOB: 1. create one empty command
 * 		2. link it at the end of the list
 * 		3. *cmd points to the new end.
 * 	Returns: 0 on ok, 1 on oom.
 * 	Caller: build_cmds()
 * */
static int	add_command(t_cmd **cmds, t_cmd **cmd)
{
	*cmd = cmd_new();
	if (!*cmd)
		return (1);
	cmd_add_back(cmds, *cmd);
	return (0);
}

/*	BUILDING IS HERE (pass 2)
 *	Starts with one EMPTY command, to then for each token:
 *		- WORD		-> append to current command's argv
 *		- PIPE		-> a new empty command becomes the current command
 *		- OPERATOR	-> A redir token with the NEXT token as its FILE, hence that
 *		next token is skipped too.
 *	Ex. Walkthrough: "ls -l | wc > out.txt"
 *		1. start			-> cmd1 (empty)
 *		2. [ls] [-l]		-> cmd1 argv [ls, -l]
 *		3. [T_PIPE]			-> cmd2 (empty)
 *		4. [wc]				-> cmd2 argv [wc]
 *		5. [T_OUT][out.txt]	-> cmd2 redirect [> out.txt], [out.txt] skipped
 *	Every string kept is copied so run_line can free the tokens after.
 *	Returns: 0 on ok, 1 on oom with unfinished list staying in *cmds for the
 *	caller to free it.
 *	Caller: parser()
 * */
static int	build_cmds(t_token *tokens, t_cmd **cmds)
{
	t_token	*cursor;
	t_cmd	*cmd;
	int		status;

	cursor = tokens;
	status = add_command(cmds, &cmd);
	while (status == 0 && cursor)
	{
		if (cursor->type == T_WORD)
			status = argv_append(cmd, cursor->value);
		else if (cursor->type == T_PIPE)
			status = add_command(cmds, &cmd);
		else
		{
			status = redir_append(cmd, cursor->type, cursor->next->value);
			cursor = cursor->next;
		}
	}
	return (status);
}

/*	Control unit of the parser: pass 1 then pass 2
 *	Returns: 0 on ok, 1 on oom, 2 on syntax error as per bash
 *	Caller: run_line()
 * */
int	parser(t_token *tokens, t_cmd **cmds)
{
	*cmds = NULL;
	if (check_syntax(tokens) != 0)
		return (2);
	return (build_cmds(tokens, cmds));
}
