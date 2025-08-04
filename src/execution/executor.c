/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:59:00 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 19:47:11 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* execute_single_command
	- fork
	- CHILD PROCESS
	- find_command_path()
		- if !path -> write STDERR
		- if path -> execve
			execve should not return on success.
			if failure perror, exit
*/
static void	execute_single_command(t_command *cmd, char **envp)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		path = find_command_path(cmd->cmd_args[0]);
		if (!path)
		{
			write(STDERR_FILENO, "minishell: command not found: ", 30);
			write(STDERR_FILENO, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
			write(STDERR_FILENO, "\n", 1);
			exit(127);
		}
		execve(path, cmd->cmd_args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

/* execute
	For now handling only one command, no pipes, no builtins
	- wait for ANY child process to finish (we have only one for now)
	- update the shell's last_exit_status
	- else... handling pipes here later
*/
int	execute(t_shell *shell_data, char **envp)
{
	int			status;
	t_command	*cmd;

	status = 0;
	cmd = shell_data->commands;
	if (!cmd)
		return (0);
	if (cmd && !cmd->next)
	{
		execute_single_command(cmd, envp);
		wait(&status);
		if (WIFEXITED(status))
		{
			shell_data->last_exit_status = WEXITSTATUS(status);
		}
	}
	return (shell_data->last_exit_status);
}
