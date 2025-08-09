/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:30:09 by anemet            #+#    #+#             */
/*   Updated: 2025/08/10 01:34:04 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*loop_and_check(char **path_dirs, char *command)
{
	int		i;
	char	*temp_path;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		temp_path = ft_strjoin(path_dirs[i], "/");
		full_path = ft_strjoin(temp_path, command);
		free(temp_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

/* find_command_path
	- check if command already has a path (e.g., starts with '/', './' or '../')
		return ft_strdup(cmd) or NULL otherwise
	- get PATH and split it
*/
char	*find_command_path(char *command)
{
	char	**path_dirs;
	char	*full_path;
	char	*path_var;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	path_var = getenv("PATH");
	if (!path_var)
		return (NULL);
	path_dirs = ft_split(path_var, ':');
	if (!path_dirs)
		return (NULL);
	full_path = loop_and_check(path_dirs, command);
	free_tokens(path_dirs);
	return (full_path);
}
