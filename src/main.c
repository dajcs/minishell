/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:34:51 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 19:34:48 by anemet           ###   ########.fr       */
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
void	shell_loop(char **envp)
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
			print_command_list(shell_data.commands);

		execute(&shell_data, envp);

		free(line);
		if (raw_tokens)
			free_tokens(raw_tokens);
		if (final_tokens)
			free_tokens(final_tokens);
		if (shell_data.commands)
			free_command_list(shell_data.commands);
	}
}

/* main
	- passing envp to shell_loop
*/
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	shell_loop(envp);
	return (0);
}
