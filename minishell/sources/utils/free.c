#include "minishell.c"

/*
free_strarr(arr):         if arr: free each arr[i] until NULL, then free(arr)
free_tokens(tok):         while tok: next = tok->next, free(tok->value),
	free(tok), tok = next
free_redirs(r):           while r: next = r->next, free(r->file),
	free(r->hd_path), free(r), r = next
free_cmds(cmd):           while cmd: next = cmd->next, free_strarr(cmd->argv),
								free_redirs(cmd->redirs), free(cmd), cmd = next
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
