/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:25:53 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 19:43:36 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ----- Includes ----- */
# include <sys/wait.h>
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

/* ----- Enums ----- */
typedef enum e_redir_type
{
	REDIR_NONE = -1,
	REDIR_INPUT = 0,
	REDIR_OUTPUT = 1,
	REDIR_HEREDOC = 2,
	REDIR_APPEND = 3
}	t_redir_type;

/* ----- Structures ----- */

// t_redir to represent a single redirection
typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;	// The filename or delimiter
	struct s_redir	*next;
}	t_redir;

// t_command a linked list of commands with arguments and redirections
typedef struct s_command
{
	// char				*command;		// the command itself (e.g., "ls")
	char				**cmd_args;		/* full arguments array for execve
											(e.g., {"ls", "-l", NULL}) */
	t_redir				*redirections;	// A linked list of redirections
	struct s_command	*next;			// Next command in the pipeline
}	t_command;

// t_shell holds the final command list
typedef struct s_shell
{
	t_command	*commands;
	int			last_exit_status;
	// we'll add here more fields later (e.g., environment list)
}	t_shell;

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
}	t_expand;

/* ----- Function Prototypes ----- */

/* src/parsing/tokenizer.c */
char			**tokenize(const char *input);
void			free_tokens(char **tokens);

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

/* src/parsing/parser_utils.c */
char			**convert_list_to_array(t_list *list);
void			free_command_list(t_command *list_head);
t_command		*handle_pipe(t_command *cmd, t_list **arg_list);
void			free_command_list(t_command *list);

/* src/execution/path_finder.c */
char			*find_command_path(char *command);

/* src/execution/executor.c */
int				execute(t_shell *shell_data, char **envp);	// returns exit st.

#endif
