/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:12:59 by anemet            #+#    #+#             */
/*   Updated: 2025/08/06 11:51:34 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

/* signal_handler()
	The signal handler.
	Its only job is to set the global variable to the signal number it received
	The main loop will then act on this information. This is required to avoid
	using non-async-signal-safe functions inside the handler.
*/
void	signal_handler(int signum)
{
	g_signal_status = signum;
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
	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

/* set_execution_signals()
	Configures signals for when the parent shell is executing commands
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
