/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:34:51 by anemet            #+#    #+#             */
/*   Updated: 2025/08/01 15:11:58 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// The main loop of the shell
/*
  1. Read input using readline
  2. Handle Ctrl-D (readline returns NULL)
  3. Add to history if the line is not empty
  4. Tokenize the input line
  --- Testing ---
  5. Print the tokens to verify they are correct
  6. Free memory for the next loop iteration
*/
void	shell_loop(void)
{
	char	*line;
	t_shell	shell_data;

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
		shell_data.tokens = tokenize(line);
		if (shell_data.tokens)
			check_tokens(shell_data);
		free(line);
		if (shell_data.tokens)
			free_tokens(shell_data.tokens);
	}
}

// we'll setup signals here later
int	main(void)
{
	shell_loop();
	return (0);
}
