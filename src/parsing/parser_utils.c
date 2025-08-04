/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:42:04 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 15:51:25 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* handle_pipe
	- convert cmd->cmd_args list to array
	- free arg_list
	- allocate memory for the next t_command and set cmd->next to the new cmd
	- return the new cmd pointer
*/
t_command	*handle_pipe(t_command *cmd, t_list **arg_list)
{
	cmd->cmd_args = convert_list_to_array(*arg_list);
	ft_lstclear(arg_list, free);
	cmd->next = ft_calloc(1, sizeof(t_command));
	return (cmd->next);
}

// initiate empty argv of size(list + 1)
static char	**init_argv(t_list *list)
{
	size_t	count;
	t_list	*node;
	char	**argv;

	count = 0;
	node = list;
	while (node)
	{
		count++;
		node = node->next;
	}
	if (count == 0)
		return (NULL);
	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	return (argv);
}

/* convert_list_to_array
	- iterate through nodes
		- argv[i] = ft_strdup()
	- terminate argv[i] with NULL
	- return argv
*/
char	**convert_list_to_array(t_list *list)
{
	size_t	i;
	t_list	*node;
	char	**argv;

	argv = init_argv(list);
	if (!argv)
		return (NULL);
	i = 0;
	node = list;
	while (node)
	{
		argv[i] = ft_strdup((char *)node->content);
		if (!argv[i])
		{
			while (i > 0)
				free(argv[--i]);
			free(argv);
			return (NULL);
		}
		i++;
		node = node->next;
	}
	argv[i] = NULL;
	return (argv);
}

static void	free_redirs(t_redir *cur)
{
	t_redir	*next;

	while (cur)
	{
		next = cur->next;
		free(cur->filename);
		free(cur);
		cur = next;
	}
}

/*
  Frees all memory associated with a command list
  Iterates through the linked list
	- free cmd_args
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
		if (current->cmd_args)
			free_tokens(current->cmd_args);
		if (current->redirections)
			free_redirs(current->redirections);
		free(current);
		current = next;
	}
}
