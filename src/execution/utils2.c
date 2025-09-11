/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 16:37:01 by anemet            #+#    #+#             */
/*   Updated: 2025/09/11 16:39:19 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	handle_single_builtin()
	Executes a single built-in command, handling its redirections.
	This allows built-ins like `cd` and `exit` to affect the parent shell.
	It saves and restores standard I/O if redirections are present.
	Return the exit status of the dispatched built-in command.
 */
static int	handle_single_builtin(t_shell *shell_data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (handle_redirections(shell_data->commands, &saved_stdin,
			&saved_stdout) == -1)
	{
		shell_data->last_exit_status = 1;
		return (1);
	}
	status = dispatch_builtin(shell_data->commands, shell_data);
	restore_io(saved_stdin, saved_stdout);
	shell_data->last_exit_status = status;
	return (status);
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
