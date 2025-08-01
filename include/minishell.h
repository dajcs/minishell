/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:25:53 by anemet            #+#    #+#             */
/*   Updated: 2025/08/01 15:13:37 by anemet           ###   ########.fr       */
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

/* ----- Structures ----- */

// for now we're representing a token list as a simple array of strings
typedef struct s_shell
{
	char	**tokens;
	// we'll add here more fields later (e.g., the command list)
}	t_shell;

/* ----- Function Prototypes ----- */

/* src/parsing/tokenizer.c */
char	**tokenize(const char *input);

// Utility functions
void	free_tokens(char **tokens);

#endif
