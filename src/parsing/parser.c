/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:22:42 by anemet            #+#    #+#             */
/*   Updated: 2025/08/11 19:07:03 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* create_redirections
	- allocate memory
	- set redir->type
	- set redir->filename
	- return redir
*/
t_redir	*create_redirection(t_redir_type type, char *filename)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}

/* handle_redirection
	- get_redir_type
	- if there is no next token (to get the filename), return 0
	- create_redirection of type t_redir
	- if (!redir) return 0
	- add redir to the end of cmd->redirections
	- increment *i by 2 (for the redirection token and the filename)
*/
int	handle_redirection(char **tokens, int *i, t_command *cmd)
{
	t_redir_type	type;
	t_redir			*redir;
	t_redir			*tail;

	type = get_redir_type(tokens[*i]);
	if (!tokens[*i + 1])
		return (0);
	redir = create_redirection(type, tokens[*i + 1]);
	if (!redir)
		return (0);
	if (!cmd->redirections)
		cmd->redirections = redir;
	else
	{
		tail = cmd->redirections;
		while (tail->next)
			tail = tail->next;
		tail->next = redir;
	}
	*i += 2;
	return (1);
}

/*
 * handle_token()
 *
 * Processes a single token, identifying and handling it as a redirection,
	a pipe,
 * or a regular argument.
 *
 * @param tokens       The array of all tokens.
 * @param i            A pointer to the current index in the tokens array.
 * @param current_cmd  A pointer to the current command being built.

	* @param arg_list     A pointer to the list of arguments for the current
	command.
 * @return             Returns 0 on failure (e.g., redirection error),
	1 on success.
 */
static int	handle_token(char **tokens, int *i, t_command **current_cmd,
		t_list **arg_list)
{
	if (get_redir_type(tokens[*i]) != REDIR_NONE)
	{
		if (!handle_redirection(tokens, i, *current_cmd))
			return (0);
	}
	else if (ft_strncmp(tokens[*i], "|", 2) == 0)
	{
		*current_cmd = handle_pipe(*current_cmd, arg_list);
		(*i)++;
	}
	else if (*tokens[*i] != '\0')
	{
		ft_lstadd_back(arg_list, ft_lstnew(ft_strdup(tokens[*i])));
		(*i)++;
	}
	else
		(*i)++;
	return (1);
}

/*
 * process_tokens()
 *
 * Contains the main loop for token processing. It iterates through the tokens
 * and uses handle_token() to process each one.
 *
 * @param tokens       The array of tokens to process.
 * @param head         The head of the command list,
	for freeing in case of an error.
 * @param current_cmd  A pointer to the current command being built.

	* @param arg_list     A pointer to the list of arguments for the current
	command.
 * @return             Returns 0 on failure,
	1 on successful processing of all tokens.
 */
int	process_tokens(char **tokens, t_command *head, t_command **current_cmd,
		t_list **arg_list)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (!handle_token(tokens, &i, current_cmd, arg_list))
		{
			free_command_list(head);
			return (0);
		}
	}
	return (1);
}

/* parse
	initiate parsing
	- allocate memory for the first current_cmd
	- store address in head
	- call process tokens
	- if fault during token processing return NULL
	- else convert and store arg_list from the last command
	- return head
*/
t_command	*parse(char **tokens)
{
	t_command	*head;
	t_command	*current_cmd;
	t_list		*arg_list;

	arg_list = NULL;
	if (!tokens || !tokens[0])
		return (NULL);
	current_cmd = ft_calloc(1, sizeof(t_command));
	head = current_cmd;
	if (!process_tokens(tokens, head, &current_cmd, &arg_list))
		return (NULL);
	current_cmd->cmd_args = convert_list_to_array(arg_list);
	ft_lstclear(&arg_list, free);
	return (head);
}
