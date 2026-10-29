/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 11:17:45 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/29 11:27:09 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_redir
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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env			*env;
	t_cmd			*cmds;
	int				last_status;
	int				saved_in;
	int				saved_out;
}	t_shell;

typedef struct s_pipe
{
	int				prev_read;
	int				fd[2];
	pid_t			last_pid;
}	t_pipe;

typedef struct s_wscan
{
	char			*buf;
	char			**fields;
	int				touched;
	int				sq;
	int				dq;
}	t_wscan;

/* shell/shell_init.c */
int			shell_init(t_shell *sh, char **envp);

/* shell/run_line.c */
void		run_line(t_shell *sh, char *line);

/* shell/shell_exit.c */
void		shell_exit(t_shell *sh, int status);

/* utils/error.c */
void		print_error(const char *first, const char *second,
				const char *third);

/* utils/free.c */
void		free_strarr(char **arr);
void		free_tokens(t_token *token);
void		free_cmds(t_cmd *cmd);

/* utils/str_utils.c */
int			is_space(char c);
int			is_meta(char c);

/* env/env_init.c */
t_env		*env_init(char **envp);
t_env		*env_new(char *key, char *value);
void		env_add_back(t_env **list, t_env *node);
void		env_free(t_env *list);

/* env/env_access.c */
char		*env_get(t_env *env, char *key);
int			env_set(t_env **env, char *key, char *value);
void		env_unset(t_env **env, char *key);

/* env/env_array.c */
char		**env_to_array(t_env *env);

/* lexer/lexer.c */
int			lexer(char *line, t_token **tokens);

/* lexer/token.c */
t_token		*token_new(t_type type, char *value);
void		token_add_back(t_token **list, t_token *token);
const char	*token_text(t_type type);

/* parser/parser.c */
int			parser(t_token *tokens, t_cmd **cmds);

/* parser/syntax.c */
int			check_syntax(t_token *tokens);

/* parser/cmd.c */
t_cmd		*cmd_new(void);
void		cmd_add_back(t_cmd **list, t_cmd *cmd);
int			argv_append(t_cmd *cmd, char *word);
int			redir_append(t_cmd *cmd, t_type type, char *file);

/* heredoc/heredoc.c */
int			heredoc_collect(t_shell *sh);

/* heredoc/heredoc_child.c */
void		hd_child(t_shell *sh, t_redir *r);

/* heredoc/heredoc_cleanup.c */
void		heredoc_cleanup(t_cmd *cmds);

/* executor/execute.c */
int			execute(t_shell *sh);

/* executor/pipeline.c */
void		close_if_open(int fd);
int			run_pipeline(t_shell *sh);

/* executor/wait.c */
int			wait_children(pid_t last_pid, int failed);

/* executor/child.c */
void		exec_fail(t_shell *sh, char *a, char *b, int status);
void		child_process(t_shell *sh, t_cmd *cmd, t_pipe *p);

/* executor/path.c */
char		*find_in_path(t_shell *sh, char *name);
int			check_exec_path(char *name, char *path);

/* executor/redirect.c */
int			apply_redirs(t_redir *redir);

/* expander	*/
int			expand_cmds(t_shell *sh);
char		**expand_word(t_shell *sh, char *raw);
char		*read_var(t_shell *sh, char *raw, int *i);
int			field_close(t_wscan *st);
int			field_emit(t_wscan *st, char c, int quoted);
char		*char_to_str(char c);
char		*join_free(char *a, char *b);
char		*remove_quotes(char *s);
char		*expand_string(t_shell *sh, char *s);

/* builtins	*/
int			is_builtin(char *name);
int			run_builtin(t_shell *sh, char **argv);
int			bt_echo(char **argv);
int			bt_cd(t_shell *sh, char **argv);
int			bt_pwd(t_shell *sh);
int			bt_export(t_shell *sh, char **argv);
int			bt_unset(t_shell *sh, char **argv);
int			bt_env(t_shell *sh);
int			bt_exit(t_shell *sh, char **argv);
char		*build_new_pwd(char *old_pwd, char *target);
int			push_component(char ***stack, char *seg);
int			print_export(t_shell *sh);
t_env		**sorted_env(t_env *env);
int			is_valid_identifier(char *s);

/* signals/signal.c */
void		set_sig_int_quit(void (*on_sigint)(int), void (*on_sigquit)(int));
void		set_signals_prompt(void);
void		set_signals_ignore(void);
void		set_signals_default(void);
void		set_signals_heredoc(void);

#endif
