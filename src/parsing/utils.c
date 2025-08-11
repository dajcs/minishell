/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:17:30 by anemet            #+#    #+#             */
/*   Updated: 2025/08/11 20:51:53 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* get_redir_type()
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
	printf("------------------------------\n\n");
}

// --- Helper function for printing a single command ---
void	print_single_command(t_command *cmd, int cmd_num)
{
	int		j;
	t_redir	*redir;

	printf("--- cmd #%d ---\n", cmd_num);
	if (cmd->cmd_args)
	{
		printf("cmd_args: ");
		j = 0;
		while (cmd->cmd_args[j])
		{
			printf("[%s] ", cmd->cmd_args[j]);
			j++;
		}
		printf("\n");
	}
	redir = cmd->redirections;
	while (redir)
	{
		printf("redir   : TYPE=%d, FILENAME=%s\n", redir->type,
			redir->filename);
		redir = redir->next;
	}
}

// --- Helper function for testing ---
void	print_command_list(t_command *cmd_list)
{
	int	i;

	i = 1;
	while (cmd_list)
	{
		print_single_command(cmd_list, i++);
		cmd_list = cmd_list->next;
		if (cmd_list)
			printf("            | (pipe)\n");
	}
	printf("----------------------------------------\n\n");
}

void	pprint(t_shell *shell, char **raw, char **final, int exit_status)
{
	if (!shell->debug)
		return ;
	if (!exit_status)
	{
		if (strrchr(shell->debug, 'r'))
			print_tokens(raw, "raw_tokens");
		if (strrchr(shell->debug, 'f'))
			print_tokens(final, "final_tokens");
		if (strrchr(shell->debug, 'c'))
			print_command_list(shell->commands);
	}
	else if (strrchr(shell->debug, 'x'))
		printf("\nlast_exit_status = %d\n\n", shell->last_exit_status);
}
