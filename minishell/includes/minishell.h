#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

# define HD_PREFIX "/tmp/.minishell_hd_"

extern volatile sig_atomic_t	g_signal;

typedef enum e_type
{
	T_WORD,
	T_PIPE,
	T_IN,
	T_OUT,
	T_APPEND,
	T_HEREDOC
}	t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef	struct	s_redir
{
	t_type			type;
	char			*file;
	char			*hd_path;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_env			*env;
	t_cmd			*cmds;
	int				last_status;
}	t_shell;

typedef struct s_pipe
{
	int				prev_read;
	int				fd[2];
	pid_t			last_pid;
}	t_pipe;

/*	utils/error.c	*/
void		print_error(const char *first, const char *second, const char *third);
/*	utils/free.c	*/
void		free_strarr(char **arr);
void		free_toens(ttoken *token);
void		free_redirs(t_redir *redir);
void		free_cmds(t_cmd *cmd);
/*	utils/str_utils.c	*/
int			is_space(char c); //is also in libft
int			is_meta(char c);
/*	lexer/lexer.c	*/
int			lexer(char *line, t_token **tokens);
/*	lexer/token.c	*/
t_token		*token_new(t_type type, char *value);
void		token_add_back(t_token **list, t_token *token);
const char	*token_text(t_type type);
/*	parser/parser.c	*/
int			parser(t_token *tokens, t_cmd **cmds);
/*	parser/syntax.c	*/
int			syntax_error(t_token *token);
int			check_syntax(t_token *tokens);
/*	parser/cmd.c	*/
t_cmd		*cmd_new(void);
void		cmd_add_back(t_cmd **list, t_cmd *cmd);
int			argv_append(t_cmd *cmd, char *word);
int			redir_append(t_cmd *cmd, t_type type, char *file);
/*	executor/execute.c	*/
int			execute(t_shell *sh);
int			run_builtin_parent(t_shell *sh, t_cmd *cmd);
/*	executor/pipeline.c	*/
void		close_if_open(int fd);
int			spawn_one(t_shell *sh, t_cmd *cmd, t_pipe *p);
int			run_pipeline(t_shell *sh);
/*	executor/wait.c	*/
int			wait_children(pid_t last_pid, int failed);
/*	executor/child.c	*/
void		exec_fail(t_shell *sh, char *a, char *b, int status);
void		exec_external(t_shell *sh, char **argv);
void		child_process(t_shell *sh, t_cmd *cmd, t_pipe *p);
/*	execuutor/path.c	*/
char		*find_in_path(t_shell *sh, char *name);
int			check_exec_path(char *name, char *path);
/*	executor/redirect.c	*/
int			apply_redirs(t_redir *redir);
int			open_redirs(t_redir *redir);
/*	env	*/
/*	shell	*/
/*	heredoc	*/

#endif
