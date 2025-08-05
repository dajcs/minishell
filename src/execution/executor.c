/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:59:00 by anemet            #+#    #+#             */
/*   Updated: 2025/08/05 14:25:18 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(const char *command)
{
	if (!command)
		return (0);
	if (ft_strncmp(command, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(command, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(command, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(command, "env", 4) == 0)
		return (1);
	if (ft_strncmp(command, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(command, "export", 7) == 0)
		return (1);
	if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	return (0);
}

/* dispatch_builtin()
	- Checks if the command is a built-in and executes it if so
		returns the built-in's exit code
		(builtin_exit returns 255 -> exit will be handled by shell_loop)
	- Returns -1 if not built-in
*/
static int	dispatch_builtin(t_shell *shell_data, char **envp)
{
	char	**args;

	args = shell_data->commands->cmd_args;
	if (args == NULL || args[0] == NULL)
		return (-1);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (builtin_echo(args));
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (builtin_cd(args));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (builtin_pwd(args));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (builtin_env(envp));
	if (ft_strncmp(args[0], "exit", 5) == 0)
		return (builtin_exit(args));
	return (-1);
}

/* execute_single_command
	- fork
	- CHILD PROCESS
	- find_command_path()
		- if !path -> write STDERR
		- if path -> execve
			execve should not return on success.
			if failure perror, exit
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
*/

/* execute_external_command()
	- fork
	- pid == 0: CHILD PROCESS
		- handle redirections for child (we don't need to save/restore
			since this process will be replaced by execve)
			- exit if redirection fails
		- find_command_path()
			- if !path -> write STDERR
			- if path -> execve
				execve should not return on success.
				if failure perror, exit
	- else: PARENT PROCESS
		- wait for the child to finish
		- the rest of status handling done in the main `execute()`
*/
static void	execute_external_command(t_command *cmd, char **envp)
{
	pid_t	pid;
	char	*path;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		if (handle_redirections(cmd, NULL, NULL) == -1)
			exit(EXIT_FAILURE);
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
	else
	{
		waitpid(pid, &status, 0);
	}
}

/* execute
	For now handling only one command
	BUILTINS
	- redirect the main shell process temporarily
		- if redir fail -> set last_exit_status failure -> return 1
	- execute builtin
	- restore io
	- set last_exit_status
	EXTERNAL COMMAND
	- redirection will be handled inside the child process
	- set last_exit_status
*/
int	execute(t_shell *shell_data, char **envp)
{
	int			status;
	t_command	*cmd;
	int			saved_stdin;
	int			saved_stdout;

	status = 0;
	saved_stdin = -1;
	saved_stdout = -1;
	cmd = shell_data->commands;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[0])
		return (0);
	if (cmd && !cmd->next)
	{
		if (is_builtin(cmd->cmd_args[0]))
		{
			if (handle_redirections(cmd, &saved_stdin, &saved_stdout) == -1)
			{
				shell_data->last_exit_status = 1;
				return (1);
			}
			status = dispatch_builtin(shell_data, envp);
			restore_io(saved_stdin, saved_stdout);
			shell_data->last_exit_status = status;
		}
		else
		{
			execute_external_command(cmd, envp);
			wait(&status);
			if (WIFEXITED(status))
				shell_data->last_exit_status = WEXITSTATUS(status);
		}
	}
	return (shell_data->last_exit_status);
}
