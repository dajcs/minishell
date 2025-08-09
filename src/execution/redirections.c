/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 11:38:45 by anemet            #+#    #+#             */
/*   Updated: 2025/08/09 10:27:36 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc(const char *delimiter)
{
	int		pipe_fds[2];
	char	*line;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
	close(pipe_fds[1]);
	return (pipe_fds[0]);
}

/* redir_error()
	reduce handle_redirections() line count under 25
*/
static int	redir_error(char *filename)
{
	perror(filename);
	return (-1);
}

/* handle_redirections()
	Sets up all I/O redirections for a given command.
*/
int	handle_redirections(t_command *cmd)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIR_INPUT)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_OUTPUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = handle_heredoc(redir->filename);
		if (fd == -1)
			return (redir_error(redir->filename));
		if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
