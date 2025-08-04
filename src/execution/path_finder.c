/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:30:09 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 16:49:20 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* find_command_path
	- check if command already has a path (e.g., starts with '/', './' or '../')
		if executable return ft_strdup(cmd) or NULL otherwise
	- get PATH and split it
*/
char *find_command_path(char *command)
{
	char **path_dirs;
	char *full_path;
	char *path_var;
	int i;

	if (ft_strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return NULL;
	}
	return NULL;
}
