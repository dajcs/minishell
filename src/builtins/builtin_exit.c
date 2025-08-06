/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:31:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/06 14:08:17 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_exit
	- return our special exit code 255
*/
int	builtin_exit(char **args)
{
	(void)args;
	return (EXIT_BUILTIN_CODE);
}
