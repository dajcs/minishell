/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:34:51 by anemet            #+#    #+#             */
/*   Updated: 2025/09/11 10:40:21 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**duplicate_envp(char **envp)
{
	int		count;
	char	**new_envp;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_tokens(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

/* free_loop_resources
	- free all
	- set shell->commands = NULL to prevent double free on next loop
*/
void	free_loop_resources(t_shell *shell, char *line, char **raw,
		char **final)
{
	if (line)
	{
		free(line);
		line = NULL;
	}
	if (raw)
		free_tokens(raw);
	if (final)
		free_tokens(final);
	if (shell->commands)
		free_command_list(shell->commands);
	shell->commands = NULL;
}

/* process_line()
	Handles the processing of a single line of input.
	- Adds the line to the history.
	- Tokenizes the line.
	- Expands and cleans the tokens.
	- Parses the tokens into commands.
	- Executes the commands.
	- Frees the resources used for the current line.
	Return The exit status of the executed command.
*/
static int	process_line(t_shell *shell_data, char *line)
{
	char	**raw_tokens;
	char	**final_tokens;
	int		exit_status;

	raw_tokens = NULL;
	final_tokens = NULL;
	add_history(line);
	raw_tokens = tokenize(line);
	if (raw_tokens)
		final_tokens = expand_and_clean(raw_tokens, shell_data);
	if (final_tokens)
		shell_data->commands = parse(final_tokens);
	pprint(shell_data, raw_tokens, final_tokens, 0);
	exit_status = execute(shell_data);
	pprint(shell_data, raw_tokens, final_tokens, 1);
	free_loop_resources(shell_data, line, raw_tokens, final_tokens);
	return (exit_status);
}

/* shell_loop()
	The main loop of the shell.
	WHILE (1)
	- Sets interactive signals before reading a new line.
	- Reads input using readline().
	- If readline returns NULL (Ctrl-D pressed), breaks the loop.
	- If the line has content, it is processed.
	- The exit status from processing is checked to determine
	if the shell should exit.
	- If the line is empty, it is freed and the loop continues.
*/
void	shell_loop(t_shell *shell_data)
{
	int		exit_status;
	char	*line;

	while (1)
	{
		set_interactive_signals();
		line = readline("minishell> ");
		if (!line)
			break ;
		if (*line)
		{
			exit_status = process_line(shell_data, line);
			if (exit_status >= EXIT_BUILTIN_CODE)
			{
				shell_data->last_exit_status = exit_status - EXIT_BUILTIN_CODE;
				break ;
			}
		}
		else
			free(line);
	}
}

/* main
	- minishell can be started with "rfcx" debug string argument:
		- r: print raw tokens
		- f: print final tokens
		- c: print parsed command
		- x: print exit status
	- shell_data.envp_list = duplicate_envp()
	- passing shell_data with our own envp to shell_loop
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell_data;

	if (argc > 1)
		shell_data.debug = argv[1];
	else
		shell_data.debug = NULL;
	shell_data.commands = NULL;
	shell_data.last_exit_status = 0;
	shell_data.envp_list = duplicate_envp(envp);
	if (!shell_data.envp_list)
		return (1);
	shell_loop(&shell_data);
	free_tokens(shell_data.envp_list);
	return (shell_data.last_exit_status);
}
