/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:38:45 by anemet            #+#    #+#             */
/*   Updated: 2025/08/11 16:31:48 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_redirection(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_INPUT)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_OUTPUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		perror(redir->filename);
		return (-1);
	}
	if (redir->type == REDIR_INPUT)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

/* handle_redirections()
	Sets up all I/O redirections for a given command.
	- If 'saved_stdin' or 'saved_stdout' are not NULL, saves the original FDs
		This is needed for built-ins.
	- For external commands in a child process we don't need save/restore,
		so we can pass NULL.
*/
int	handle_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
	t_redir	*redir;

	if (saved_stdin)
		*saved_stdin = dup(STDIN_FILENO);
	if (saved_stdout)
		*saved_stdout = dup(STDOUT_FILENO);
	redir = cmd->redirections;
	while (redir)
	{
		if (setup_redirection(redir) == -1)
		{
			if (saved_stdin && *saved_stdin != -1)
				restore_io(*saved_stdin, -1);
			if (saved_stdout && *saved_stdout != -1)
				restore_io(-1, *saved_stdout);
			return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

/* restore_io()
	Restores stdin and/or stdout from saved file descriptors
*/
void	restore_io(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
