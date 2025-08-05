/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:12:59 by anemet            #+#    #+#             */
/*   Updated: 2025/08/05 18:19:26 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_status = 0;

/* signal_handler()
	The signal handler.
	Its only job is to set the global variable to the signal number it received
	It then reinvokes itself for the same signal.
*/
void	signal_handler(int signum)
{
	g_signal_status = signum;
}
