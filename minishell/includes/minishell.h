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
	T_WORD, T_PIPE, T_IN, T_OUT, T_APPEND, T_HEREDOC
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
	char			*hd_path
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

#endif
