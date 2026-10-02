#include "minishell.c"

/*
free_strarr(arr):         if arr: free each arr[i] until NULL, then free(arr)
free_tokens(tok):         while tok: next = tok->next, free(tok->value),
	free(tok), tok = next
free_redirs(r):           while r: next = r->next, free(r->file),
	free(r->hd_path), free(r), r = next
free_cmds(cmd):           while cmd: next = cmd->next, free_strarr(cmd->argv),
								free_redirs(cmd->redirs), free(cmd), cmd = next
Rule used in the whole project: every malloc'ed thing has exactly one
owner, and exactly one function here that frees it. If valgrind reports
a leak of a token, a redir or a cmd, the bug is either a missing call
to one of these functions, or a field that was malloc'ed somewhere but
is not freed here.
What these functions do NOT do:
     - They do not set the caller's pointer to NULL. A function cannot
       change the caller's variable when it receives a copy of the
       pointer. So after free_cmds(sh->cmds) the caller must write
       sh->cmds = NULL itself (run_line does this). Otherwise sh->cmds
       still points to freed memory, and freeing it again later
       (for example in shell_exit) is a double free.
     - free_redirs does not delete heredoc temp files (see free_redirs).
All of them accept NULL and then do nothing, so they are safe to call
on an empty list.
*/
/*
 * Frees a NULL terminated string array and every string in it.
 * Called by: free_cmds, find_in_path, exec_external, env_to_array.
 */
void	free_strarr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

/*
 * After free(token), reading token->next is reading freed memory. So the 
 * address of the next token is copied into next_token first, then the current
 * token is freed, then we move on to the saved address.
*/
void	free_tokens(t_token *token)
{
	t_token	*next_token;

	while (token != NULL)
	{
		next_token = token->next;
		free(token->value);
		free(token);
		token = next_token;
	}
}

/*A redir owns two strings:
 *     - file:    the filename, or the heredoc delimiter for <<.
 *     - hd_path: the heredoc temp file path, NULL for <, > and >>.
 *
 * Important: this frees the hd_path STRING only. It does NOT delete the
 * temp file on disk (no unlink). Deleting is done by heredoc_cleanup,
 * called by run_line in the parent after all children have finished.
 * WHY: free_redirs also runs inside child processes (through
 * shell_exit -> free_cmds). If a child deleted the temp files when it
 * exits, another child of the same pipeline that has not opened its
 * heredoc yet would find the file missing.
 * */
void	free_redirs(t_redir *redir)
{
	t_redir	*next_redir;

	while (redir != NULL)
	{
		next_redir = redir->next;
		free(redir->file);
		free(redir->hd_path);
		free(redir);
		redir = next_redir;
	}
}

/* Each cmd owns:
 *     - argv:   a NULL-terminated array of strings -> free_strarr
 *     - redirs: a list of redirections             -> free_redirs
 *
 * Example: "cat < in | wc -l" is two cmds:
 *     cmd 1: argv [ "cat" ],       redirs: < in
 *     cmd 2: argv [ "wc", "-l" ],  redirs: none (NULL)
 * free_cmds frees cmd 1's argv and redirs, then cmd 1, then the same
 * for cmd 2.
 * */
void	free_cmds(t_cmd *cmd)
{
	t_cmd	*next_cmd;

	while (cmd != NULL)
	{
		next_cmd = cmd->next;
		free_strarr(cmd->argv);
		free_redirs(cmd->redirs);
		free(cmd);
		cmd = next_cmd;
	}
}
/* Pseudo code

# Frees the whole token list from the lexer, called by: run_line and lexer
	free_tokens(token);
		while (token);
			next = token->next;
			free(token->vlaue);
			token = next;

# Frees the redirection list. Also frees hd_path string but without unlinking file
# Called by free_cmds;
	free_redirs(r);
		while (r);
			next = r->next;
			free(r->file);
			free(r->hd_path);
			free(r);
			r = next;

# Frees the whole command list of one line. Called by: run_line, shell_exit.
	free_cmds(cmd);
		while (cmd);
			next = cmd->next;
			free_strarr(cmd->argv);
			free_redirs(cmd->redirs);
			free(cmd);
			cmd = next;
*/
