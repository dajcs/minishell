/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:59:00 by anemet            #+#    #+#             */
/*   Updated: 2025/08/09 10:07:59 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(const char *command)
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
static int	dispatch_builtin(t_command *cmd, t_shell *shell_data)
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

/* run_command()
	Runs ONE command, either a built-in or an external.
	Designed to be called from within a forked child process.
	It handles its own redirections and then executes.
	- handle redirections / exit if redirection fails
	- if builtin execute it and exit child with its status
	- else (external command -- this part never returns on success)
		- if !path - write error, exit(127)
		- execve() -- should not return, if returns perror("execve") / exit(F)
*/
static int	run_command(t_command *cmd, t_shell *shell_data)
{
	char	*path;
	int		status;

	if (handle_redirections(cmd, NULL, NULL) == -1)
		exit(EXIT_FAILURE);
	if (is_builtin(cmd->cmd_args[0]))
	{
		status = dispatch_builtin(cmd, shell_data);
		exit(status);
	}
	path = find_command_path(cmd->cmd_args[0]);
	if (!path)
	{
		write(STDERR_FILENO, "minishell: command not found: ", 30);
		write(STDERR_FILENO, cmd->cmd_args[0], ft_strlen(cmd->cmd_args[0]));
		write(STDERR_FILENO, "\n", 1);
		exit(127);
	}
	execve(path, cmd->cmd_args, shell_data->envp_list);
	perror("execve");
	exit(EXIT_FAILURE);
}

/* execute()
	The main executor.
	It handles redirections and pipelines of any lenght
	- set_execution_signals() -> parent to ignore signals Ctrl-\ and Ctrl-C
	- special case: single built-in command (not in pipe)
		this allows `cd` and `exit` to modify parent shell
		- set_interactive_signals() and return last_exit_status
	while(cmd) PIPELINE EXECUTION LOOP
	- if(cmd->next) this isn't the last command: create a pipe
	- fork()
	--- CHILD PROCESS ---
	- connect prev_pipe_read_end -> STDIN_FILENO
	- if cmd->next connect new pipe write pipe_fds[1] -> STDOUT_FILENO
	- run_command() -- should not return / or it will exit on failure
	--- PARENT PROCESS ---
	- close(prev_pipe_read_end) // Parent is done with the old pipe
	- if(cmd->next)
		- close new pipe write pipe_fds[1] // Parent doesn't write to new pipe
		- prev_pipe_read_end = pipe_fds[0] //save new pipe read for next child
	WAIT for ALL CHILDREN to FINISH
	- while(wait(NULL) > 0) // ignore intermediate statuses
	- waitpid(last_pid, &status, 0) // get the exit status of the LAST command
	- set_interactive_signals()
	- if "normal" exit (WIFEXITED): set last_exit_status
	- else if terminated by signal (WIFSIGNALED)
		- set last_exit_status
		- if SIGINT (Ctrl-C) print new prompt
		- else if SIGQUIT (Ctrl-\) print "Quit (core dumped)"
	- return last_exit_status
*/
int	execute(t_shell *shell_data)
{
	t_command	*cmd;
	int			pipe_fds[2];
	int			prev_pipe_read_end;
	pid_t		last_pid;
	int			status;
	int			saved_stdin;
	int			saved_stdout;
	int			final_status;
	pid_t		exited_pid;
	int			termsig;

	prev_pipe_read_end = -1;
	status = 0;
	final_status = 0;
	cmd = shell_data->commands;
	if (!cmd)
		return (0);
	set_execution_signals();
	if (!cmd->next && is_builtin(cmd->cmd_args[0]))
	{
		if (handle_redirections(cmd, &saved_stdin, &saved_stdout) == -1)
		{
			shell_data->last_exit_status = 1;
			return (1);
		}
		status = dispatch_builtin(cmd, shell_data);
		restore_io(saved_stdin, saved_stdout);
		shell_data->last_exit_status = status;
		set_interactive_signals();
		return (status);
	}
	while (cmd)
	{
		if (cmd->next)
			if (pipe(pipe_fds) == -1)
				return (perror("pipe"), 1);
		last_pid = fork();
		if (last_pid == -1)
			return (perror("fork"), 1);
		if (last_pid == 0)
		{
			set_child_signals();
			if (prev_pipe_read_end != -1)
			{
				dup2(prev_pipe_read_end, STDIN_FILENO);
				close(prev_pipe_read_end);
			}
			if (cmd->next)
			{
				close(pipe_fds[0]);
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
			}
			run_command(cmd, shell_data);
		}
		if (prev_pipe_read_end != -1)
			close(prev_pipe_read_end);
		if (cmd->next)
		{
			close(pipe_fds[1]);
			prev_pipe_read_end = pipe_fds[0];
		}
		cmd = cmd->next;
	}
	while ((exited_pid = wait(&status)) > 0) // loop until wait() returns -1
											// meaning no more children to wait
	{
		if (exited_pid == last_pid)
			final_status = status;
	}
	set_interactive_signals();
	if (WIFEXITED(final_status))
		shell_data->last_exit_status = WEXITSTATUS(final_status);
	else if (WIFSIGNALED(final_status))
	{
		termsig = WTERMSIG(final_status);
		shell_data->last_exit_status = 128 + termsig;
		if (termsig == SIGINT)
			printf("\n");
		else if (termsig == SIGQUIT)
			printf("Quit (core dumped)\n");
	}
	return (shell_data->last_exit_status);
}
