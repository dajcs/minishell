/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:22:42 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 15:47:17 by anemet           ###   ########.fr       */
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

/*
  The main parsing function for parsing / step 4
  It takes the token list and creates a linked list of t_command structs
  1. Handle edge cases (no tokens or empty input)
  2. Allocate memory for current_cmd ( struct t_command )
  3. Make the head point to this cmd in linked list t_command (next to be set)
  3. Populate the current_cmd struct's fields from token array (while (tokens))
	a. check for redirection ('<' '>' '<<' '>>'), if redirection:
		- create a new t_redir node
		- set its type
		- set its filename (from tokens[i+1])
		- add it to current_cmd->redirections list
		- next token i+2 (skip over operator and filename)
	b. check for pipe '|', if pipe:
		- finalize the current command's cmd_args for the temporary arg_list
		- reset arg_list for the next command
		- allocate memory for the next command and store its pointer in next
		- make that command current_cmd
		- next token i++
	c. else - it is an "ordinary" argument
		- add a copy of tokens[i] to the temporary arg_list
		- next token i++
  4. Out of tokens:
		- finalize the last command's cmd_args
		- clear temp arg_list
  5. Return head
*/
// t_command	*parse(char **tokens)
// {
// 	t_command		*head;
// 	t_command		*current_cmd;
// 	t_list			*arg_list;
// 	t_redir_type	type;
// 	t_redir			*redir;
// 	t_redir			*tail;
// 	int				i;

// 	i = 0;
// 	arg_list = NULL;
// 	if (!tokens || !tokens[0])
// 		return (NULL);
// 	current_cmd = ft_calloc(1, sizeof(t_command));
// 	head = current_cmd;
// 	while (tokens[i])
// 	{
// 		type = get_redir_type(tokens[i]);
// 		if (type != REDIR_NONE)
// 		{
// 			redir = ft_calloc(1, sizeof(t_redir));
// 			if (!redir)
// 			{
// 				free_command_list(head);
// 				return (NULL);
// 			}
// 			redir->type = type;
// 			if (!tokens[i + 1])
// 			{
// 				free(redir);
// 				free_command_list(head);
// 				return (NULL);
// 			}
// 			redir->filename = ft_strdup(tokens[i + 1]);
// 			if (!redir->filename)
// 			{
// 				free(redir);
// 				free_command_list(head);
// 				return (NULL);
// 			}
// 			if (!current_cmd->redirections)
// 				current_cmd->redirections = redir;
// 			else
// 			{
// 				tail = current_cmd->redirections;
// 				while (tail->next)
// 					tail = tail->next;
// 				tail->next = redir;
// 			}
// 			i += 2;
// 		}
// 		else if (ft_strncmp(tokens[i], "|", 2) == 0)
// 		{
// 			current_cmd->cmd_args = convert_list_to_array(arg_list);
// 			ft_lstclear(&arg_list, free);
// 			current_cmd->next = ft_calloc(1, sizeof(t_command));
// 			current_cmd = current_cmd->next;
// 			i++;
// 		}
// 		else
// 		{
// 			ft_lstadd_back(&arg_list, ft_lstnew(ft_strdup(tokens[i])));
// 			i++;
// 		}
// 	}
// 	current_cmd->cmd_args = convert_list_to_array(arg_list);
// 	ft_lstclear(&arg_list, free);
// 	return (head);
// }

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
		else
		{
			ft_lstadd_back(arg_list, ft_lstnew(ft_strdup(tokens[i])));
			i++;
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
