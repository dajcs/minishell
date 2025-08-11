/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:03:00 by anemet            #+#    #+#             */
/*   Updated: 2025/08/11 17:30:56 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// typedef struct s_fname
// {
// 	char		*base;
// 	char		*pid_str;
// 	char		*count_str;
// 	char		*temp;
// 	char		*filename;
// }	t_fname;
static char	*create_temp_filename(void)
{
	static int	counter;
	t_fname		f;

	f.base = "/tmp/minishell_heredoc_";
	f.pid_str = ft_itoa(getpid());
	if (!counter)
		counter++;
	f.count_str = ft_itoa(-counter++);
	f.temp = ft_strjoin(f.base, f.pid_str);
	f.filename = ft_strjoin(f.temp, f.count_str);
	free(f.pid_str);
	free(f.count_str);
	free(f.temp);
	return (f.filename);
}

static void	write_heredoc_to_fd(int fd, const char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static int	process_one_heredoc(t_redir *redir)
{
	char	*delim;
	char	*tempfile;
	int		fd;

	delim = redir->filename;
	tempfile = create_temp_filename();
	fd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc temp file");
		free(tempfile);
		return (-1);
	}
	write_heredoc_to_fd(fd, delim);
	close(fd);
	free(redir->filename);
	redir->filename = tempfile;
	redir->type = REDIR_INPUT;
	return (0);
}

int	process_heredocs(t_command *commands)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (process_one_heredoc(redir) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	cleanup_heredocs(t_command *commands)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->filename && ft_strncmp(redir->filename,
					"/tmp/minishell_heredoc_", 23) == 0)
				unlink(redir->filename);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
