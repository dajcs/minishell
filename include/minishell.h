/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:25:53 by anemet            #+#    #+#             */
/*   Updated: 2025/08/03 16:41:25 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ----- Includes ----- */
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

/* ----- Defines ----- */
# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2

/* ----- Structures ----- */

typedef struct s_command
{
	char				*command;		// the command itself (e.g., "ls")
	char				**arguments;	/* full arguments array for execve
										(e.g., {"ls", "-l", NULL}) */
	int					input_fd;		// for now STDIN_FILENO
	int					output_fd;		// for now STDOUT_FILENO
	struct s_command	*next;			// for now will always be NULL
}				t_command;

// t_shell holds the final command list
typedef struct s_shell
{
	t_command	*commands;
	int			last_exit_status;
	// we'll add here more fields later (e.g., environment list)
}				t_shell;

// t_expand members put in a struct because of 25 lines
typedef struct s_expand
{
	int		alen;			// length of token after var
	int		vlen;			// variable length
	char	*dollar;		// pointer to $ char in a token
	char	*var;			// var string
	char	*var_val;		// var expanded to var_val string
	char	*before_var;	// token string before the $ sign
	char	*after_var;		// token string after the var
	char	*tmp_token;		// tmp string: before_val + var_val
	char	*new_token;		// new_token: before_val + var_val + after_var
}				t_expand;

/* ----- Function Prototypes ----- */

/* src/parsing/tokenizer.c */
char			**tokenize(const char *input);

/* src/parsing/tokenizer_utils.c */
int				is_metachar(const char *s);
int				is_quotechar(const char *s);
int				check(int (*checker_func)(const char *), const char *s, int *i);
char			*extract_if(const char *s, int *i,
					int (*checker_func)(const char *));
void			end_varchar(const char *s, int *i);

/* src/parsing/expander.c */
char			**expand_and_clean(char **tokens, int last_exit_status);

/* src/parsing/parser.c */
t_command		*parse(char **tokens);

// Utility functions
void			free_tokens(char **tokens);
void			free_command_list(t_command *list);

#endif
