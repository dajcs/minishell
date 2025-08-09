/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:22:42 by anemet            #+#    #+#             */
/*   Updated: 2025/08/10 00:44:50 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* get_redir_type
	- return redirection type of the token
	- or return REDIR_NONE if there is no token, or it is not a redirection
*/
t_redir_type	get_redir_type(char *token)
{
	if (ft_strncmp(token, "<", 2) == 0)
		return (REDIR_INPUT);
	if (ft_strncmp(token, ">", 2) == 0)
		return (REDIR_OUTPUT);
	if (ft_strncmp(token, "<<", 3) == 0)
		return (REDIR_HEREDOC);
	if (ft_strncmp(token, ">>", 3) == 0)
		return (REDIR_APPEND);
	return (REDIR_NONE);
}

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

/* process_tokens
	contains the main loop for token processing
	- check if token is redirection & handle_redirection if it is
	- check if token is a pipe & handle_pipe if it is
	- otherwise it's an "ordinary" token, just add it to the arg_list
*/
int	process_tokens(char **tokens, t_command *head, t_command **current_cmd,
		t_list **arg_list)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (get_redir_type(tokens[i]) != REDIR_NONE)
		{
			if (!handle_redirection(tokens, &i, *current_cmd))
			{
				free_command_list(head);
				return (0);
			}
		}
		else if (ft_strncmp(tokens[i], "|", 2) == 0)
		{
			*current_cmd = handle_pipe(*current_cmd, arg_list);
			i++;
		}
		else if (*tokens[i] != '\0')
		{
			ft_lstadd_back(arg_list, ft_lstnew(ft_strdup(tokens[i])));
			i++;
		}
		else
			i++;
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
