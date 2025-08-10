/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:25:53 by anemet            #+#    #+#             */
/*   Updated: 2025/08/10 16:55:38 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ----- Includes ----- */
# define _GNU_SOURCE
# include <signal.h>			// sig_atomic_t
# include <sys/wait.h>			// waitpid
# include <sys/stat.h>			// struct stat (path_stat)
# include <linux/limits.h>		// PATH_MAX 4096
# include <readline/history.h>
# include <readline/readline.h>
# include <fcntl.h>				// open() and its flags
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

/* ----- Defines ----- */
# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2
# define EXIT_BUILTIN_CODE 256

/* ----- Enums ----- */
typedef enum e_redir_type
{
	REDIR_NONE = -1,
	REDIR_INPUT = 0,
	REDIR_OUTPUT = 1,
	REDIR_HEREDOC = 2,
	REDIR_APPEND = 3
}	t_redir_type;

/* ----- Globals ----- */
// extern volatile sig_atomic_t	g_signal_status;

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
	char		**envp_list;			// our private modifiable env list
	int			last_exit_status;
	int			debug;					// used to print parsed commands
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
int				is_quotechar_plus(const char *s);
int				check(int (*checker_func)(const char *), const char *s, int *i);
char			*extract_if(const char *s, int *i,
					int (*checker_func)(const char *));

/* src/parsing/expander.c */
char			*get_env_value(const char *var_name, t_shell *shell_data);
char			**expand_and_clean(char **tokens, t_shell *shell_data);

/* src/parsing/parser.c */
t_command		*parse(char **tokens);

/* src/parsing/parser_utils.c */
char			**convert_list_to_array(t_list *list);
void			free_command_list(t_command *list_head);
t_command		*handle_pipe(t_command *cmd, t_list **arg_list);
void			free_command_list(t_command *list);

/* src/parsing/env_utils.c */
char			**duplicate_envp(char **envp);

/* src/execution/path_finder.c */
char			*find_command_path(char *command);

/* src/execution/executor.c */
int				execute(t_shell *shell_data);	// returns exit status

/* src/execution/signals.c */
void			set_interactive_signals(void);
void			set_execution_signals(void);
void			set_child_signals(void);

/* src/execution/redirections.c */
void			restore_io(int saved_stdin, int saved_stdout);
int				handle_redirections(t_command *cmd, int *saved_stdin,
					int *saved_stdout);

/* ----- Built-in Functions (src/builtins) ----- */
int				builtin_echo(char **args);
int				builtin_cd(t_shell *shell, char **args);
int				expand_tilde_path(char *dest, const char *src, t_shell *shell);
int				handle_home_path(char *dest, t_shell *shell);
int				builtin_pwd(t_shell *shell, char **args);
int				builtin_env(char **args);
int				builtin_exit(char **args);
int				builtin_unset(t_shell *shell, char **args);
int				builtin_export(t_shell *shell, char **args);
int				find_var_index(char **envp, char *var_name);
void			set_env_var(t_shell *shell, char *var_assignment);
int				is_valid_identifier(const char *name);
char			*get_var_name(const char *assignment);

#endif
