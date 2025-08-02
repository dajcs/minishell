/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:22:42 by anemet            #+#    #+#             */
/*   Updated: 2025/08/02 11:19:51 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/*
  A helper function to duplicate a NULL-terminated array of strings
  It is a "deep-copy": it allocates new memory for the array and for each
  string within it.
  The original `tokens` array will be freed, so we need to have our own copies.
*/
static char	**duplicate_string_array(char **array)
{
	int		i;
	int		len;
	char	**new_array;

	len = 0;
	while (array[len])
		len++;
	new_array = malloc(sizeof(char *) * (len + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_array[i] = ft_strdup(array[i]);
		if (!new_array[i])
		{
			free_tokens(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

/*
  The main parsing function for parsing / step 2
  It takes the token list and creates a single t_command struct
  1. Handle edge cases (no tokens or empty input)
  2. Allocate memory for the struct t_command cmd
  3. Populate the struct's fields
		The command is the first token. We need our own copy (ft_strdup)
		The arguments are a DEEP COPY of the entire token array
  4. File descriptors initialized to standard IO for now
  5. No pipes yet, next command is NULL
*/
t_command	*parse(char **tokens)
{
	t_command	*cmd;

	if (!tokens || !tokens[0])
		return (NULL);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->command = ft_strdup(tokens[0]);
	if (!cmd->command)
	{
		free(cmd);
		return (NULL);
	}
	cmd->arguments = duplicate_string_array(tokens);
	if (!cmd->arguments)
	{
		free(cmd->command);
		free(cmd);
		return (NULL);
	}
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

/*
  Frees all memory associated with a command list
  Iterates through the linked list
	- free command
	- free arguments
	- free the struct itself
*/
void	free_command_list(t_command *list_head)
{
	t_command	*current;
	t_command	*next;

	current = list_head;
	while (current != NULL)
	{
		next = current->next;
		if (current->command)
			free(current->command);
		if (current->arguments)
			free_tokens(current->arguments);
		free(current);
		current = next;
	}
}
