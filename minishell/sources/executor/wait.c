#include "minishell.h"

/*
 * Converts a raw wait() status into a shell exit status.
 * Returns: exit code, or 128 + signal number if killed by signal.
 * Called by: wait_children.
 * */
static int	decode_status(int ws)
{
	if (WIFEXITED(ws))
		return (WEXITSTATUS(ws));
	if (WIFSIGNALED(ws))
		return (128 + WTERMSIG(ws));
	return (1);
}

/*
 * Prints what bash prints when the last command dies from a signal:
 * CTRL-C => newline
 * CTRL-\ => "Quit"
 * Caller is wait_children()
 * */
static void	report_signal(int ws)
{
	if (!WIFSIGNALED(ws))
		return ;
	if (WTERMSIG(ws) == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (WTERMSIG(ws) == SIGQUIT && WCOREDUMP(ws))
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	else if (WTERMSIG(ws) == SIGQUIT)
		write(STDERR_FILENO, "Quit\n", 5);
}

/*
 * Reaps ALL of the children of the pipeline: only last_pid decides the status.
 * failed = 1 means a pipe or fork failed,
	and last_pid in not the real last command
 * and the status is forced to 1.
 * Returns: status for $?.
 * Called by: run_pipeline.
 * */
int	wait_children(pid_t last_pid, int failed)
{
	pid_t	pid;
	int		ws;
	int		last_ws;

	last_ws = 0;
	pid = wait(&ws);
	while (pid != -1)
	{
		if (pid == last_pid)
			last_ws = ws;
		pid = wait(&ws);
	}
	if (failed || last_pid == -1)
		return (1);
	report_signal(last_ws);
	return (decode_status(last_ws));
}
