#include "minishell.h"

/*
 * Closes fd unless it is -1 (the "no pipe here" marker in t_pipe).
 * Called by: spawn_one, run_pipeline.
 */
void	close_if_open(int fd)
{
	if (fd != -1)
		close(fd);
}

/*
 * Creates the pipe to the next command if any, forks one child, then closes
 * the parent's copies of the pipe ends that are no longer needed.
 * Updates: p->prev_read and p->last_pid for the next call.
 * Returns: 0 on ok, 1 on either pipe() or fork() fail.
 * Called by: run_pipeline, and once per command.
 * */
static int	spawn_one(t_shell *sh, t_cmd *cmd, t_pipe *p)
{
	pid_t	pid;

	p->fd[0] = -1;
	p->fd[1] = -1;
	if (cmd->next && pipe(p->fd) == -1)
	{
		print_error("pipe", strerror(errno), NULL);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		print_error("fork", strerror(errno), NULL);
		close_if_open(p->fd[0]);
		close_if_open(p->fd[1]);
		return (1);
	}
	if (pid == 0)
		child_process(sh, cmd, p);
	close_if_open(p->prev_read);
	close_if_open(p->fd[1]);
	p->prev_read = p->fd[0];
	p->last_pid = pid;
	return (0);
}

/*
 * Runs every command of: sh->cmds in its own child, connected by pipes.

	* The Parent ignores ctrl-C while it waits (PLACE_HOLDER_SET_SIGNALS_IGNORE_FUNC)
 * 		- if it sets SIGINT and SIGQUIT to SIG_IN: ctrl-c during a command will
 * 		only kill the children, the shell reports 130 and survives.
 * 		- if it leaves the prompt handler in place: a ctrl-c would also run that
 * 		handler in the shell, redrawing the prompt mid-command.
 * Returns: exit status of last command, which becomes $?
 * Called by: execute
 * */
int	run_pipeline(t_shell *sh)
{
	t_pipe	p;
	t_cmd	*cmd;

	p.prev_read = -1;
	p.last_pid = -1;
	PLACE_HOLDER_SET_SIGNALS_IGNORE_FUNC();
	// for the parent to ignore ctrl-c while waiting.
	cmd = sh->cmds;
	while (cmd)
	{
		if (spawn_one(sh, cmd, &p) != 0)
			break ;
		cmd = cmd->next;
	}
	close_if_open(p.prev_read);
	return (wait_children(p.last_pid, cmd != NULL));
}

/* PSEUDOCODE: caller of this file (executor/execute.c)

# Entry point of the executor. Returns the status for $?.
# Called by: run_line.
execute(sh):
	cmd = sh->cmds
	if cmd->next == NULL and cmd->argv[0] and is_builtin(cmd->argv[0]):
		return run_builtin_parent(sh, cmd)     # lone builtin, no fork
	return run_pipeline(sh)
*/
