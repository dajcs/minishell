/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:25:53 by anemet            #+#    #+#             */
/*   Updated: 2025/08/02 11:22:12 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ----- Includes ----- */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ----- Defines ----- */
# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2

/* ----- Structures ----- */

typedef struct s_command
{
	char				*command; // the command itself (e.g., "ls")
	char				**arguments; /* full arguments array for execve
												(e.g., {"ls", "-l", NULL}) */
	int					input_fd; // for now STDIN_FILENO
	int					output_fd; // for now STDOUT_FILENO
	struct s_command	*next; // for now will always be NULL
}	t_command;

// for now we're representing a token list as a simple array of strings
// typedef struct s_shell
// {
// 	char	**tokens;
// 	// we'll add here more fields later (e.g., the command list)
// }	t_shell;

// update t_shell to hold the final command list, not the raw tokens
typedef struct s_shell
{
	t_command	*commands;
	// we'll add here more fields later (e.g., environment list)
}	t_shell;

/* ----- Function Prototypes ----- */

/* src/parsing/tokenizer.c */
char	**tokenize(const char *input);

/* src/parsing/parser.c */
t_command	*parse(char **tokens);

// Utility functions
void	free_tokens(char **tokens);
void	free_command_list(t_command *list);

#endif
