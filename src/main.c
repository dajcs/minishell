/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:34:51 by anemet            #+#    #+#             */
/*   Updated: 2025/08/07 15:45:48 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(char **tokens, char *token_type)
{
	int	i;

	printf("--- %s ---\n", token_type);
	printf("---\n");
	i = 0;
	while (tokens[i])
	{
		printf("  [%d]: \"%s\"\n", i, tokens[i]);
		i++;
	}
	printf("-------------------------------------\n\n");

}

// --- Helper function for testing ---
void	print_command_list(t_command *cmd_list)
{
	int		i;
	int		j;
	t_redir	*redir;

	i = 1;
	while (cmd_list)
	{
		printf("--- Command #%d ---\n", i++);
		if (cmd_list->cmd_args)
		{
			printf("cmd_args: ");
			j = 0;
			while (cmd_list->cmd_args[j])
			{
				printf("[%s] ", cmd_list->cmd_args[j]);
				j++;
			}
			printf("\n");
		}
		redir = cmd_list->redirections;
		while (redir)
		{
			printf("  Redirection: TYPE=%d, FILENAME=%s\n", redir->type,
				redir->filename);
			redir = redir->next;
		}
		cmd_list = cmd_list->next;
		if (cmd_list)
			printf("  | (pipe to next command)\n");
	}
	printf("---------------------------------\n");
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

// The main loop of the shell
/*
	- Initialize shell data
	WHILE (1)
	- set_interactive_signals() before reading a new line
	- Read input using readline()
	- if (!line)
		- (Ctrl-D pressed): break loop, clear memory, exit minishell
	- if (*line) -> we got a line with content
		- Add to history if the line is not empty
		- Tokenize (gets raw tokens with quotes)
		- Expand and Clean (expands $, removes quotes)
		- Parse (creates t_command from clean tokens)
		- launch executor, capture exit_status
		- if exit_status == EXIT_BUILTIN_CODE: `exit` command has been entered
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
			// print_tokens(raw_tokens, "raw_tokens");
			if (raw_tokens)
				final_tokens = expand_and_clean(raw_tokens, shell_data);
			else
				final_tokens = NULL;
			if (final_tokens)
			{
				// print_tokens(final_tokens, "final_tokens");
				shell_data->commands = parse(final_tokens);
				// print_command_list(shell_data->commands);
			}
			exit_status = execute(shell_data);
			if (exit_status == EXIT_BUILTIN_CODE)
			{
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

	(void)argc;
	(void)argv;
	shell_data.commands = NULL;
	shell_data.last_exit_status = 0;
	shell_data.envp_list = duplicate_envp(envp);
	if (!shell_data.envp_list)
		return (1);
	shell_loop(&shell_data);
	free_tokens(shell_data.envp_list);
	return (shell_data.last_exit_status);
}
