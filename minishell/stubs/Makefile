# stubs/Makefile - TEST BUILD ONLY, lives on the "stubs" branch, never merged.
# Usage (from minishell/):   make -C stubs        -> ./stubs/minishell_stubs
# Swap a stub for the real module once it lands:
#   make -C stubs re REAL="expander"   (or REAL="expander builtins signals")
# Use "re" whenever REAL changes, so the binary is relinked.
# Then delete that stub_*.c from STUB_SRCS for good.

NAME		= minishell_stubs
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g3 -O0
ROOT		= ..
LIBFT_DIR	= $(ROOT)/libft
LIBFT		= $(LIBFT_DIR)/libft.a
LIBFT_REPO	= https://github.com/basq42/Libft.git
LIBFT_REF	= master
INCS		= -I$(ROOT)/includes -I$(LIBFT_DIR)
LDLIBS		= -L$(LIBFT_DIR) -lft -lreadline

REAL		?=
# Baran's modules: always the real code.
BARAN_DIRS	= env executor heredoc lexer parser shell utils
BARAN_SRCS	= $(foreach d,$(BARAN_DIRS),$(wildcard $(ROOT)/sources/$(d)/*.c))
# Bahtiyor's modules: real code only if listed in REAL, else the stub.
REAL_SRCS	= $(foreach d,$(REAL),$(wildcard $(ROOT)/sources/$(d)/*.c))
# One stub per module: stub_main.c, stub_signals.c, stub_expander.c,
# stub_builtins.c. Listing "main" in REAL drops stub_main.c, and so on.
STUB_SRCS	= $(filter-out $(addprefix stub_,$(addsuffix .c,$(REAL))), \
				stub_main.c stub_signals.c stub_expander.c stub_builtins.c)
ifneq ($(filter main,$(REAL)),)
REAL_SRCS	+= $(ROOT)/sources/main.c
endif

SRCS		= $(BARAN_SRCS) $(REAL_SRCS) $(STUB_SRCS)
OBJ_DIR		= obj
OBJS		= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@

$(LIBFT): $(LIBFT_DIR)/Makefile
	$(MAKE) -C $(LIBFT_DIR)

# Same libft/ as the real build (../libft): cloned only if it is missing,
# without .git, so it can be committed on master and merged in here.
$(LIBFT_DIR)/Makefile:
	git clone --depth 1 --branch $(LIBFT_REF) $(LIBFT_REPO) $(LIBFT_DIR)
	rm -rf $(LIBFT_DIR)/.git

$(LIBFT_DIR)/libft.h: | $(LIBFT_DIR)/Makefile

$(OBJ_DIR)/%.o: %.c $(ROOT)/includes/minishell.h $(LIBFT_DIR)/libft.h \
		| $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

vg: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes \
		--track-fds=yes --suppressions=readline.supp ./$(NAME)

.PHONY: all clean fclean re vg
