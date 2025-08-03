/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:22:42 by anemet            #+#    #+#             */
/*   Updated: 2025/08/03 20:25:57 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return redirection type of the token, or -1 if not token is not redirection
t_redir_type	get_redir_type(char *token)
{
	if (ft_strcmp(token, "<") == 0)
		return (REDIR_INPUT);
	if (ft_strcmp(token, ">") == 0)
		return (REDIR_OUTPUT);
	if (ft_strcmp(token, "<<") == 0)
		return (REDIR_HEREDOC);
	if (ft_strcmp(token, ">>") == 0)
		return (REDIR_APPEND);
	return -1;
}

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
  The main parsing function for parsing / step 4
  It takes the token list and creates a list of t_command structs
  1. Handle edge cases (no tokens or empty input)
  2. Allocate memory for the struct t_command cmd
  3. put it on the head of t_command list
  3. Populate the struct's fields
		The command is the first token. We need our own copy (ft_strdup)
		The arguments are a DEEP COPY of the entire token array
  4. File descriptors initialized to standard IO for now
  5. No pipes yet, next command is NULL
*/
t_command	*parse(char **tokens)
{
	t_command	*head;
	t_command	*current_cmd;
	t_list		*arg_list;
	t_redir_type type;
	int			i;

	i = 0;
	arg_list = NULL;

	if (!tokens || !tokens[0])
		return (NULL);
	current_cmd = ft_calloc(1, sizeof(t_command));
	head = current_cmd;
	while (tokens[i])
	{
		type = get_redir_type(tokens[i]);
		if (type != -1) // it's a redirection
		{
			/*
			- create a new t_redir node
			- set its type
			- set its filename (from tokens[i+1])
			- add it to current_cmd->redirections list
			*/
			i += 2; // Skip over the operator and filename
		}
		else if (ft_strcmp(tokens[i], "|") == 0) // it's a pipe
		{
			// 1. finalize the current command's arguments from arg_list
			current_cmd->arguments = convert_list_to_array(arg_list);
			ft_lstclear(&arg_list, free); // reset for next command
			// 2. create and link the next command
			current_cmd->next = ft_calloc(1, sizeof(t_command));
			current_cmd = current_cmd->next;
			i++;
		}
		else // it's normal argument
		{
			// add tokens[i] to the temporary arg_list
			ft_lstadd_back(&arg_list, ft_lstnew(ft_strdup(tokens[i])));
			i++;
		}
	}
	// Finalize the last command's arguments
	current_cmd->arguments = convert_list_to_array(arg_list);
	ft_lstclear(&arg_list, free);
	return (head);
}

/*
	TODO: update 
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
