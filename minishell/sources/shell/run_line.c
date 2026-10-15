#include "minishell.h"

/*	One command line, from text to $?
 *	run_line takes one line typed at the prompt and passes it through every
 *	stage in order. Each stage only reads the output of the stage before it:
 *		line	--lexer---------->	tokens
 *		tokens	--parser--------->	sh->cmds (tokens freed)
 *		cmds	--heredoc_collect->	tem files for each <<
 *		cmds	--expander-------->	$VAR, $? expanded, quotes removed
 *		cmds	--execute--------->	status of the line becomes $? then the temp
 *		files are deleted and sh->cmds is freed so the shell is clean for the
 *		next line!
 *	CONDITIONALS ON IMPLEMENTATION:
 *		- on my code, run_line is the only places that writes sh->last_status,
 *		ie $?
 *		- DEPENDED ON MAIN IMPLEMTATION: as of now, main is expected to set it
 *		to 130 after a ctrl-C at the prompt (g_signal) and read it for ctrl-D
 * */
/*
 * When a stage before execution (lexer, parser or expander) fails:
 * 	- if out of memory	-> print it here as the failed stage wont.
 * 	- if syntax error	-> the lexer or parser already prints the msg.
 * 	As per bash, sets the $? to that status and frees whatever was built in
 * 	commands as the parser can leave a semi built list on out of memory case.
 * 	shell_exit() frees the non-null sh->cmds, so setting to = NULL here to
 * 	prevent double free the same list.
 * 	Caller: run_line
 * */
static void	report_stage_error(t_shell *sh, int status)
{
	if (status == 1)
		print_error("out of memory", NULL, NULL);
	sh->last_status = status;
	free_cmds(sh->cmds);
	sh->cmds = NULL;
}

/*
 * After a succesfull parse, for a line that has commands:
 * 	1. heredoc_collect: reads every << body into its respective temp file:
 * 		1.1. Returns: 0 on ok, 1 on error, 130 ctrl-c. On error or ctrl-c
 * 		nothing else runs and the status is set to $? with msg already printed.
 *
 * 	2. PLACE_HOLDER_EXPAND_CMDS_FUNC(sh): should return 0 on ok, 1 on oom, TELL
 * 	ME IF YOU MAKE THE RETURNS DIFFERENT + REFER TO DEPENDS ON below.
 *
 * 	3. execute: runs the commands and sets status to $?
 * 	4. IN ANY CASE:
 * 		4.1. Delete the temp files
 * 		4.2. Free the commands
 * 		4.3. set sh->cmds = NULL for respective functions.
 * 	Why Call Heredoc BEFORE expander: as per Bash: bash reads heredocs while it
 * 	reads the line before any expansion; and the heredoc needs the RAW DELIMITER
 * 	with its QUOTES PRESERVED, to know whether the body needs to be expanded.
 * 	CLEANUP ONLY HERE: as at this point all the children finished so no process
 * 	awaits for a temp file.
 * 	DEPENDS ON: expander/ for what it returns:
 * 		- 0: everything expanded, execute runs
 * 		- 1: "out of memory" is printed here, $? = 1
 * 		- any other non-zero value: this code would still print "out of memory"
 * 		and use that value as $?
 * 		SO DECISION: whether to keep 0 / 1 only or change the print_error line
 * 	Caller: run_line()
 * */
static void	run_cmds(t_shell *sh)
{
	int	status;

	status = heredoc_collect(sh);
	if (status == 0)
	{
		status = PLACE_HOLDER_EXPAND_CMDS_FUNC(sh);
		if (status != 0)
			print_error("out of memory", NULL, NULL);
		else
			status = execute(sh);
	}
	sh->last_status = status;
	heredoc_cleanup(sh->cmds);
	free_cmds(sh->cmds);
	sh->cmds = NULL;
}

/*
 * Runs one command line inputted at the prompt.
 * STEPS:
 * 		1. lexer: translate the line into tokens. On error, returns 2 on
 * 		Unclosed Quote and 1 on oom, and lexer already freed the tokens.
 * 		2. If the line is only SPACES => NO tokens: hence nothing to do so $? is
 * 		kept unchanged (as per bash).
 * 		3. parser: parses tokens into command list. The tokens are freed after,
 * 		regardless of success or not: parser copied every string it kept.
 * 		4. On either lexer or parser error: report_stage_error sets $? to 1 or 2
 * 		5. If all good, calls run_cmds: heredocs, expansion, execution, cleanup.
 * 	MAIN SHOULD FREE THE LINE ITSELF AFTER THIS FUNCTION RETURNS.
 * 	CALLER: MAIN
 * 	run_line DOES NOT FREE LINE so
 * 	DEPENDABLE:
 * 		- if main calls run_line for every non-empty line and frees line after
 * 		it returns: no leak as each line handled once.
 * 		- if main also calls it for EMPTY LINES: harmless too as the lexer gives
 * 		no tokens and $? is kept as is.
 * */
void	run_line(t_shell *sh, char *line)
{
	t_tokens	*tokens;
	int			status;

	tokens = NULL;
	status = lexer(line, &tokens);
	if (status == 0 && !tokens)
	{
		status = parser(tokens, &sh->cmds);
		free_tokens(tokens);
	}
	if (!status)
	{
		report_stage_error(sh, status);
		return ;
	}
	if (sh->cmds)
		run_cmds(sh);
}
