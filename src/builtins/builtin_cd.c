/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:14:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/05 08:23:18 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_cd
	- if no argument cd to $HOME
	- otherwise cd to args[1]
	- perror("cd") prints: "cd: [directory]: No such file or directory"
*/
int	builtin_cd(char **args)
{
	if (args[1] == NULL)
	{
		if (chdir(getenv("HOME")) != 0)
		{
			perror("cd");
			return (1);
		}
		else
			return (0);
	}
	else if (chdir(args[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	else
		return (0);
}
