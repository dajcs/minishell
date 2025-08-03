/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:34:51 by anemet            #+#    #+#             */
/*   Updated: 2025/08/03 13:58:48 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	check_tokens(t_shell shell_data)
{
	int	i;

	i = 0;
	while (shell_data.tokens[i])
	{
		printf("  [%d]: \"%s\"\n", i, shell_data.tokens[i]);
		i++;
	}
}
*/

// --- Helper function for testing ---
void	print_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	printf("--- Command ---\n");
	printf("Command: \"%s\"\n", cmd->command);
	printf("Arguments:\n");
	if (cmd->arguments)
	{
		i = 0;
		while (cmd->arguments[i])
		{
			printf("  [%d]: \"%s\"\n", i, cmd->arguments[i]);
			i++;
		}
	}
	printf("Input FD: %d\n", cmd->input_fd);
	printf("Output FD: %d\n", cmd->output_fd);
	printf("Next command: %p\n", cmd->next);
	printf("------------------\n");
}

// The main loop of the shell
/*
  0. Initialize shell data
  1. Read input using readline
  2. Handle Ctrl-D (readline returns NULL -> exit())
  3. Add to history if the line is not empty
  4. Tokenize (gets raw tokens with quotes)
  5. Expand and Clean (expands $, removes quotes)
  5. Parse (creates t_command from clean tokens)
  5. TESTING (This will be replaced by the executor later)
  6. Free all allocated memory for this cycle
*/
void	shell_loop(void)
{
	char	*line;
	char	**raw_tokens;
	char	**final_tokens;
	t_shell	shell_data;

	shell_data.commands = NULL;
	shell_data.last_exit_status = 0;
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line && *line)
			add_history(line);
		raw_tokens = tokenize(line);
		if (raw_tokens)
			final_tokens = expand_and_clean(raw_tokens,
					shell_data.last_exit_status);
		else
			final_tokens = NULL;
		if (final_tokens)
			shell_data.commands = parse(final_tokens);
		else
			shell_data.commands = NULL;
		if (shell_data.commands)
			print_command(shell_data.commands);
		free(line);
		if (raw_tokens)
			free_tokens(raw_tokens);
		if (final_tokens)
			free_tokens(final_tokens);
		if (shell_data.commands)
			free_command_list(shell_data.commands);
	}
}

// we'll setup signals here later
int	main(void)
{
	shell_loop();
	return (0);
}
