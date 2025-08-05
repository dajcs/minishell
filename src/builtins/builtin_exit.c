/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:31:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/05 09:33:40 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_exit
	- exit_code from last_exit_status
	- free all memory before exit
*/
int	builtin_exit(char **args)
{
	(void)args;
	printf("exit\n");
	return (EXIT_BUILTIN_CODE);
}
