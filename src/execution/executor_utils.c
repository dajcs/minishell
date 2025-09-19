/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 23:44:42 by anemet            #+#    #+#             */
/*   Updated: 2025/09/18 14:09:47 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

/* dispatch_builtin()
	- Checks if the command is a built-in and executes it if so
		returns the built-in's exit code
		(builtin_exit returns (256 + xcode) -> exit handled by shell_loop)
	- Returns -1 if not built-in
*/
int	dispatch_builtin(t_command *cmd, t_shell *shell_data)
{
	char	**args;

	args = cmd->cmd_args;
	if (args == NULL || args[0] == NULL)
		return (-1);
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(shell_data, args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(shell_data, args));
	if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(shell_data->envp_list));
	if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(shell_data, args));
	if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(shell_data, args));
	if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (-1);
}

/* cmd_validation()
	checks if command is a valid executable file
	- struct stat path_stat - variable holds file metadata
	- stat(path, &path_stat) - kernel fills out info about the file@path
	- S_ISDIR(path_stat.st_mode) - returns 1 if directory -> exit(126)
	- access(path, X_OK) - explicitly check for execute permission
	- perror(path) -> // perror will print "Permission denied"
*/
static void	cmd_validation(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, path, ft_strlen(path));
			write(STDERR_FILENO, ": Is a directory\n", 17);
			free(path);
			exit(126);
		}
	}
	if (access(path, X_OK) != 0)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		perror(path);
		free(path);
		exit(126);
	}
}

/*  execute_external_cmd()
	Finds and runs an external command. This function does not return on success.
	- Finds the command's full path.
	- Performs checks for command existence and validity.
	- Handles errors by printing to STDERR and exiting with the appropriate code.
	- Calls execve() to replace the current process with the new command.
	- If execve() returns, it's an error; prints it and exits.
*/
static void	execute_external_cmd(t_command *cmd, t_shell *shell_data)
{
	char	*path;

	path = find_command_path(cmd->cmd_args[0], shell_data);
	if (!path)
	{
		write(STDERR_FILENO, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit(127);
	}
	if (access(path, F_OK) != 0)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		exit(127);
	}
	cmd_validation(path);
	execve(path, cmd->cmd_args, shell_data->envp_list);
	perror("execve");
	free(path);
	exit(EXIT_FAILURE);
}

/* run_command()
	Runs a single command from within a forked child process.
	- Handles I/O redirections for the command; exits if they fail.
	- Checks if the command is a built-in. If so, it executes it and exits
		with the built-in's status code.
	- If it's not a built-in, it calls a helper function to handle
		the execution of the external command.
	- return (EXIT_FAILURE)  // This line is unreachable
*/
int	run_command(t_command *cmd, t_shell *shell_data)
{
	int	status;

	if (handle_redirections(cmd, NULL, NULL) == -1)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->cmd_args[0]))
	{
		status = dispatch_builtin(cmd, shell_data);
		exit(status);
	}
	execute_external_cmd(cmd, shell_data);
	return (EXIT_FAILURE);
}
