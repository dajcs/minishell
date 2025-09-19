/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:28:24 by anemet            #+#    #+#             */
/*   Updated: 2025/09/19 10:56:10 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	echo_write(char **args, int i)
{
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
}

/* builtin_echo
	- prints the words in args[1]... separated by a space, terminating by '\n'
	- if flag '-n' is in args[1] then it doesn't print the terminating '\n'
*/
int	builtin_echo(char **args)
{
	int	i;
	int	j;
	int	n_option;

	i = 1;
	n_option = 0;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			n_option = 1;
			i++;
		}
		else
			break ;
	}
	echo_write(args, i);
	if (!n_option)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
