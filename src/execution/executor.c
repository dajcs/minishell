/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:59:00 by anemet            #+#    #+#             */
/*   Updated: 2025/08/12 00:09:13 by anemet           ###   ########.fr       */
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

/*	execute_child_process()
	This is the entry point for the logic within a forked child process.
	It sets up signals, connects the pipes for input and output, and
	then executes the command. It does not return.
	@param cmd The command to execute.
	@param p_end The read end of the previous pipe (-1 if none).
	@param fds The file descriptors for the new pipe if one exists.
	@param data The main shell data structure.
 */
static void	execute_child_process(t_command *cmd, int p_end, int fds[2],
		t_shell *data)
{
	set_child_signals();
	if (p_end != -1)
	{
		dup2(p_end, STDIN_FILENO);
		close(p_end);
	}
	if (cmd->next)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	run_command(cmd, data);
}

/*	run_pipeline()
	Iterates through the command list, creating a pipe and forking a child
	process for each command in the pipeline.
		t_pfc (pipe-fork-child) struct:
	t_command	*cmd;
	pid_t		pid;
	int			pipe_fds[2];
	Return the process ID (PID) of the last command in the pipeline.
 */
static pid_t	run_pipeline(t_shell *shell_data, int *prev_pipe_read_end)
{
	t_pfc	p;

	p.cmd = shell_data->commands;
	p.pid = -1;
	while (p.cmd)
	{
		if (p.cmd->next)
			if (pipe(p.pipe_fds) == -1)
				return (perror("pipe"), -1);
		p.pid = fork();
		if (p.pid == -1)
			return (perror("fork"), -1);
		if (p.pid == 0)
			execute_child_process(p.cmd, *prev_pipe_read_end, p.pipe_fds,
				shell_data);
		if (*prev_pipe_read_end != -1)
			close(*prev_pipe_read_end);
		if (p.cmd->next)
		{
			close(p.pipe_fds[1]);
			*prev_pipe_read_end = p.pipe_fds[0];
		}
		p.cmd = p.cmd->next;
	}
	return (p.pid);
}

/*	wait_for_children()
	Waits for all child processes created in the pipeline to terminate.
	It then processes the exit status of the final command to set the
	shell's last_exit_status correctly, handling signals appropriately.
	last_pid: The PID of the last command's process.
 */
static void	wait_for_children(pid_t last_pid, t_shell *shell_data)
{
	int		status;
	int		final_status;
	pid_t	exited_pid;
	int		termsig;

	final_status = 0;
	while (1)
	{
		exited_pid = wait(&status);
		if (exited_pid <= 0)
			break ;
		if (exited_pid == last_pid)
			final_status = status;
	}
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
}

/* execute()
	The main execution coordinator. It prepares for execution by processing
	heredocs and then determines whether to run a single built-in command
	or a full pipeline of one or more commands.
	Return The last exit status of the command or pipeline.
 */
int	execute(t_shell *shell_data)
{
	int		status;
	pid_t	last_pid;
	int		prev_pipe_read_end;

	if (!shell_data->commands || !shell_data->commands->cmd_args
		|| !shell_data->commands->cmd_args[0])
		return (0);
	if (process_heredocs(shell_data->commands) == -1)
		return (1);
	set_execution_signals();
	if (!shell_data->commands->next
		&& is_builtin(shell_data->commands->cmd_args[0]))
		status = handle_single_builtin(shell_data);
	else
	{
		prev_pipe_read_end = -1;
		last_pid = run_pipeline(shell_data, &prev_pipe_read_end);
		if (last_pid != -1)
			wait_for_children(last_pid, shell_data);
		status = shell_data->last_exit_status;
	}
	set_interactive_signals();
	cleanup_heredocs(shell_data->commands);
	return (status);
}
