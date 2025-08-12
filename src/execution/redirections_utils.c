/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:03:00 by anemet            #+#    #+#             */
/*   Updated: 2025/08/12 16:46:39 by anemet           ###   ########.fr       */
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

/* write_heredoc_to_fd
	- check g_heredoc_interrupted before readline(),
		in case it was set while processing previous line
	- check flag immediately after readline returns
		if using advanced handler, readline will return NULL
	return -1 if g_heredoc_interrupted
	return 0 for normal operation, or Ctrl-D
*/
static int	write_heredoc_to_fd(int fd, const char *delim)
{
	char	*line;

	while (1)
	{
		if (g_heredoc_interrupted)
			return (+130);
		line = readline("> ");
		if (g_heredoc_interrupted)
		{
			free(line);
			return (+130);
		}
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
	return (0);
}

/* process_one_heredoc()
	return +1 on file open error
	return res = write_heredoc_to_fd():
		0: normal operation
		130: Ctrl-C
*/
static int	process_one_heredoc(t_redir *redir)
{
	char	*delim;
	char	*tempfile;
	int		fd;
	int		res;

	delim = redir->filename;
	tempfile = create_temp_filename();
	fd = open(tempfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc temp file");
		free(tempfile);
		return (+1);
	}
	res = write_heredoc_to_fd(fd, delim);
	close(fd);
	free(redir->filename);
	redir->filename = tempfile;
	redir->type = REDIR_INPUT;
	return (res);
}

/* process_heredocs()
	The main heredoc orchestrator. It sets up and restores signal handlers
	- original_sigint_handler = signal() // get current handler
	- signal(SIGINT, heredoc_sigint_handler_pro) // set our heredoc handler
	- g_heredoc_interrupted = 0; // reset the flag before starting
	WHILE(cmd)
	- res = process_one_heredoc()
	- if (res)  // +1 for fopen() error, +130 for Ctrl-C
		restore original_sigint_handler, return res
	After WHILE, restore original_sigint_handler, return 0
*/
int	process_heredocs(t_command *commands)
{
	t_command	*cmd;
	t_redir		*redir;
	void		(*original_sigint_handler)(int);
	int			res;

	original_sigint_handler = signal(SIGINT, SIG_DFL);
	signal(SIGINT, heredoc_sigint_handler_pro);
	g_heredoc_interrupted = 0;
	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				res = process_one_heredoc(redir);
				if (res)
				{
					signal(SIGINT, original_sigint_handler);
					return (res);
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	signal(SIGINT, original_sigint_handler);
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
