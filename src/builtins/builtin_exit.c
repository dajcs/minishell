/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:31:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/05 08:34:59 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_exit
	- exit_code from last_exit_status
	- free all memory before exit
*/
int	builtin_exit(t_shell *shell_data, char **args)
{
	int	exit_code;

	(void)args;
	printf("exit\n");
	exit_code = shell_data->last_exit_status;
	free_command_list(shell_data->commands);
	exit(exit_code);
}
