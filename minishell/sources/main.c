/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 14:30:07 by bkelav            #+#    #+#             */
/*   Updated: 2026/10/28 17:19:10 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Main routine:
 * 		1. Set the prompt's signal behaviours on each iteration.
 * 			- ctrl-c redraws prompt
 * 			- ctrl-\ is ignored
 * 			1.2: each iteration as run_line (and run_pipeline, heredoc_one) sets
 * 			the signals to ignore before fork() and doesn't revert that.
 * 		2. readline() handles the prompt display "minishell$", waits and returns
 * 		the typed line as MALLOC'd (ENTER or NULL on ctrl-D)
 * 			2.2: ctrl-c during readline: readline does not return, rather waits:
 * 			kernel runs the handler on top of the readline call, it does:
 * 					- store 2 in g_signal;
 * 					- write a newline;
 * 					- rl_on_new_line() report to readline that the cursor is on
 * 					a new line;
 * 					- rl_replace_line("", 0) empties the semi-typed text and
 * 					rl_redisplay() prints a fresh prompt.
 * 		3. sh.last_status is a local variable of main, so the handler has no
 * 		access to it. THUS THE NEED TO USE A GLOBAL VARIABLE (g_signal). This
 * 		was the handler and main can 'communicate' regarding signals.
 * 			- minishell$ ls -l^C
 * 			- minishell$ echo $?
 * 			- 130
 * 			main checks g_singla == SIGINT, yes-> sh.last_status = 130, g_signal
 * 			= 0; then run_line runs, expander should replace $? with
 * 			sh->last_status.
 * 		4. NULL = ctrl-D on empty line, print exit and end shell
 * 		5. Any non-empty line goes to history and then run_line.
 * 	Who writes the 130:
ctrl-C pressed during… | Shell SIGINT |	Who receives | Where 130 is set
------------------------------------------------------------------------
the prompt (readline)  |prompt handler|the shell only|main g_signal on readline
a command			     |ignore	      |shell + child |wait_children=(128
						+ 2)
a heredoc				|ignore		   |shell
							+ H 		|hd_wait-heredoc_collect-run_cmds
 * */

static int	is_blank_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (line[i] == '\0');
}

/*	Helper to:
 *	1. set prompt signal behaviours
 *	2. readline blocks untill: enter, ctrl-d, ctrl-c
 *	3. if ctrl-c came during wait: $? = 130
 *		3.1. Check before NULL check: ctrl-c/d must still exit 130
 *	4. NULL -> ctrl-d (EOF): print "exit" and quit.
 *	5. non emty line to history and is run.
 * */
static void	prompt_once(t_shell *sh)
{
	char	*line;

	set_signals_prompt();
	line = readline("minishell$ ");
	if (g_signal == SIGINT)
	{
		sh->last_status = 130;
		g_signal = 0;
	}
	if (line == NULL)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		shell_exit(sh, sh->last_status);
	}
	if (line[0] != '\0')
	{
		if (!is_blank_line(line))
			add_history(line);
		run_line(sh, line);
	}
	free(line);
}

/*	rl_catch_signals = 0 to avoid readline handling its own handlers */
int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	if (shell_init(&sh, envp) != 0)
	{
		print_error("out of memory", NULL, NULL);
		return (1);
	}
	while (1)
		prompt_once(&sh);
	return (0);
}
