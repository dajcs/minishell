/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:31:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/08 18:00:54 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	no_digits(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

/* builtin_exit
	- return our special exit code 256 + atoi(args[1])
*/
int	builtin_exit(char **args)
{
	int	xcode;

	if (!args[1])
		return (EXIT_BUILTIN_CODE);
	xcode = ft_atoi(args[1]);
	if (xcode == 0 && no_digits(args[1]))
	{
		write(STDERR_FILENO, "minishell: exit: ", 17);
		write(STDERR_FILENO, args[1], ft_strlen(args[1]));
		write(STDERR_FILENO, ": numeric argument required\n", 28);
		return (EXIT_BUILTIN_CODE + 2);
	}
	if (args[1] && args[2] != NULL)
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	xcode = xcode % 256;
	return (EXIT_BUILTIN_CODE + xcode);
}
