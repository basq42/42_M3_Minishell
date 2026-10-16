#include "minishell.h"

/*	ALL error messages go through print_error()
 *
 *	Format messages same as bash:
 *		minishell: first: second: third\n
 *	Parts that are NULL are skipped, so the below works:
 *		print_error("ls_typo", "command not found", NULL);
 *			-> "minishell: ls_typo: command not found"
 *		print_error("cd", "/nonono", "No such file or directory");
 *			-> "minishell: cd: /nonono: No such file or directory"
 *		print_error(file, strerror(errno), NULL);
 *			-> "minishell: out.txt: Permission denied"
 *

	* Approach: The whole message is first built then printed with one write() Because:
 *		In a pipeline several child processes run in paralel, and all write to

			*		fd 2. If each part is printed seperatly with several write() then parts of
 *		message can interleave and leave us at the mercy of the CPU:
 *			"minishell: minishell: a: b: command not found\ncommand not found"
 * */
/*
 * Adds ': ' and part to the end of message.
 * Example: message = "minishell"
 *			part = "cd"
 *			returns: "minishell: cd"
 * part can be a string literal or result of strerror()
 * */
static char	*join_part(const char *message, const char *part)
{
	char	*message_with_colon;
	char	*new_message;

	message_with_colon = ft_strjoin(message, ": ");
	free(message);
	if (message_with_colon == NULL)
		return (NULL);
	new_message = ft_strjoin(message_with_colon, part);
	free(message_with_colon);
	return (new_message);
}

/*
 * Builds the full line=> "minishell: first: second: third\n"
 * if a part is NULL, its skipped
 * */
static char	*build_message(const char *fir, const char *sec, const char *thrd)
{
	char	*message;
	char	*message_with_newline;

	message = ft_strdup("minishell");
	if (message != NULL && fir != NULL)
		message = join_part(message, fir);
	if (message != NULL && sec != NULL)
		message = join_part(message, sec);
	if (message != NULL && thrd != NULL)
		message = join_part(message, thrd);
	if (message == NULL)
		return (NULL);
	message_with_newline = ft_strjoin(message, "\n");
	free(message);
	return (message_with_newline);
}

/*
 * USED FOR WHEN MALLOC FAIL IN build_message().
 * Hence, outputs the message piece by piece with several write()
 * Issue: chance of messages interleaving. As the malloc fail can only happen
 * rarely on a system running out of memory, in which case no more mallocs.
 * So overall, not the best but better than printing nothing in my opinion.
 * */
static void	write_parts(const char *first, const char *second,
		const char *third)
{
	write(2, "minishell", 9);
	if (first != NULL)
	{
		write(2, ": ", 2);
		write(2, first, ft_strlen(first));
	}
	if (second != NULL)
	{
		write(2, ": ", 2);
		write(2, second, ft_strlen(second));
	}
	if (third != NULL)
	{
		write(2, ": ", 2);
		write(2, third, ft_strlen(third));
	}
	write(2, "\n", 1);
}

/*
 * Steps:
 *	1. Build the whole line with build_message();
 *	2. If build_message() fails,
		try printing pieces with write_parts() and stop;

		*	3. Otherwise write the whole line to stderr with one write() call and free it.
 *
 *	Does not free anything else (callers should handle)
 *	Does not set exit status (Callers should decide)
 * */
void	print_error(const char *first, const char *second, const char *third)
{
	char	*message;

	message = build_message(first, second, third);
	if (message == NULL)
	{
		write_parts(first, second, third);
		return ;
	}
	write(2, message, ft_strlen(message));
	free(message);
}
