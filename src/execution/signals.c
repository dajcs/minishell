/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:12:59 by anemet            #+#    #+#             */
/*   Updated: 2025/08/07 08:51:08 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// volatile sig_atomic_t	g_signal_status = 0;

/* interactive_signal_handler()
	Handling signals during the interactive phase (entering command at prompt)
	- (void)signum; // it's SIGINT <- set @ set_interactive_signals()
	- write("\n"); // print a newline
	- rl_on_new_line(); // tell readline that we moved on to a new line
	- rl_replace_line("", 0); // clear the current buffer in readline
	- rl_redisplay(); // redisplay the prompt on the new line;
*/
void	interactive_signal_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* set_interactive_signals()
	Configures signals for when the shell is waiting for input at the prompt.
	- SIGINT (Ctrl-C): Handled by our signal_handler().
		CRITICAL: sa_flags = 0, so the blocking readline() call is interrupted
	- SIGQUIT (Ctrl-\): Ignored
*/
void	set_interactive_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = interactive_signal_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/* set_execution_signals()
	Configures signals for the parent shell when executing the commands
		and waiting for child processes to finish
	The parent should not be terminated by signals intended for the child
	- SIGINT (Ctrl-C): Ignored by the parent
	- SIGQUIT (Ctrl-\): Ignored by the parent
*/
void	set_execution_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/* set_child_signals(void)
	Resets signal handlers for a child process to be the default OS behavior.
	At fork() the child processes inherit the parent signal handling
	(SIGINT and SIGQUIT ignored), and we're restoring the default (SIG_DFL)
*/
void	set_child_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
