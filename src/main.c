/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:34:51 by anemet            #+#    #+#             */
/*   Updated: 2025/08/11 18:29:01 by anemet           ###   ########.fr       */
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

/* shell_loop()
	The main loop of the shell
	- Initialize shell data
	WHILE (1)
	- set_interactive_signals() before reading a new line
	- Read input using readline()
	- if (!line)
		- (Ctrl-D pressed): break loop, return
	- if (*line) -> we got a line with content
		- Add to history if the line is not empty
		- Tokenize (gets raw tokens with quotes)
		- Expand and Clean (expands $, removes quotes)
		- Parse (creates t_command from clean tokens)
		- exit_status = execute(shell_data)
		- if exit_status >= EXIT_BUILTIN_CODE: `exit` command has been entered
			- Free all allocated memory and break loop (exit minishell)
		- Free all allocated memory for this cycle, and repeat
	- else free(line) - free empty line "" and new loop
*/
void	shell_loop(t_shell *shell_data)
{
	int		exit_status;
	char	*line;
	char	**raw_tokens;
	char	**final_tokens;

	raw_tokens = NULL;
	final_tokens = NULL;
	while (1)
	{
		set_interactive_signals();
		line = readline("minishell> ");
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			raw_tokens = tokenize(line);
			if (raw_tokens)
				final_tokens = expand_and_clean(raw_tokens, shell_data);
			if (final_tokens)
				shell_data->commands = parse(final_tokens);
			pprint(shell_data, raw_tokens, final_tokens, 0);
			exit_status = execute(shell_data);
			pprint(shell_data, raw_tokens, final_tokens, 1);
			if (exit_status >= EXIT_BUILTIN_CODE)
			{
				shell_data->last_exit_status = exit_status - EXIT_BUILTIN_CODE;
				free_loop_resources(shell_data, line, raw_tokens, final_tokens);
				break ;
			}
			free_loop_resources(shell_data, line, raw_tokens, final_tokens);
		}
		else
			free(line);
	}
}

/* main
	- shell_data.envp_list = duplicate_envp()
	- passing shell_data with our own envp to shell_loop
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell_data;

	(void)argv;
	shell_data.debug = NULL;
	if (argc > 1)
		shell_data.debug = argv[1];
	shell_data.commands = NULL;
	shell_data.last_exit_status = 0;
	shell_data.envp_list = duplicate_envp(envp);
	if (!shell_data.envp_list)
		return (1);
	shell_loop(&shell_data);
	free_tokens(shell_data.envp_list);
	return (shell_data.last_exit_status);
}
