NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCS		= -Iincludes -I$(LIBFT_DIR)
LDLIBS		= -L$(LIBFT_DIR) -lft -lreadline

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a
LIBFT_REPO	= https://github.com/basq42/Libft.git
LIBFT_REF	= master
HEADER		= includes/minishell.h

SRC_DIR		= sources
OBJ_DIR		= obj

# ======= Baran	======= #

ENV			= env/env_access.c env/env_array.c env/env_init.c
EXECUTOR	= executor/child.c executor/execute.c executor/path.c \
			  executor/pipeline.c executor/redirect.c executor/wait.c
HEREDOC		= heredoc/heredoc.c heredoc/heredoc_child.c \
			  heredoc/heredoc_cleanup.c
LEXER		= lexer/lexer.c lexer/token.c
PARSER		= parser/cmd.c parser/parser.c parser/syntax.c
SHELL_SRC	= shell/run_line.c shell/shell_exit.c shell/shell_init.c
UTILS		= utils/error.c utils/free.c utils/str_utils.c

# ===== Bahktiyor ======== #

MAIN		= main.c
SIGNALS		= signals/signal.c signals/signal_set.c
EXPANDER	= expander/expand_cmds.c expander/expand_field.c \
			  expander/expand_strbuf.c expander/expand_var.c \
			  expander/expand_word.c expander/quotes.c
BUILTINS	= builtins/builtins_b.c builtins/cd.c builtins/cd_path.c \
			  builtins/cd_stack.c builtins/echo.c builtins/env.c \
			  builtins/exit.c builtins/export.c builtins/export_print.c \
			  builtins/export_sort.c builtins/ident.c builtins/pwd.c \
			  builtins/unset.c

SRCS		= $(MAIN) $(ENV) $(EXECUTOR) $(HEREDOC) $(LEXER) $(PARSER) \
			  $(SHELL_SRC) $(UTILS) $(SIGNALS) $(EXPANDER) $(BUILTINS)
OBJS		= $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))

all: $(NAME)
 
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)
 
$(LIBFT): $(LIBFT_DIR)/Makefile
	$(MAKE) -C $(LIBFT_DIR)

$(LIBFT_DIR)/Makefile:
	git clone --depth 1 --branch $(LIBFT_REF) $(LIBFT_REPO) $(LIBFT_DIR)
	rm -rf $(LIBFT_DIR)/.git
 
$(LIBFT_DIR)/libft.h: | $(LIBFT_DIR)/Makefile

libft_update:
	rm -rf $(LIBFT_DIR)
	$(MAKE) $(LIBFT_DIR)/Makefile
 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) $(LIBFT_DIR)/libft.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	if [ -f $(LIBFT_DIR)/Makefile ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi
	rm -rf $(OBJ_DIR)
 
fclean: clean
	if [ -f $(LIBFT_DIR)/Makefile ]; then $(MAKE) -C $(LIBFT_DIR) fclean; fi
	rm -f $(NAME)
 
re: fclean all
 
.PHONY: all clean fclean re libft_update
