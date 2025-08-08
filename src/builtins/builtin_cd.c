/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:14:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/08 16:36:22 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* int	builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	*old_pwd_val;
	char	new_pwd_buffer[PATH_MAX];
	char	*old_pwd_export_str;
	char	*new_pwd_export_str;

	path = args[1];
	if (!path)
	{
		path = get_env_value("HOME", shell);
		if (!path)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	old_pwd_val = get_env_value("PWD", shell);
	if (chdir(path) != 0)
	{
		perror(path);
		return (1);
	}
	if (getcwd(new_pwd_buffer, sizeof(new_pwd_buffer)) == NULL)
	{
		perror("cd: getcwd error");
		return (1);
	}
	old_pwd_export_str = ft_strjoin("OLDPWD=", old_pwd_val);
	new_pwd_export_str = ft_strjoin("PWD=", new_pwd_buffer);
	set_env_var(shell, old_pwd_export_str);
	set_env_var(shell, new_pwd_export_str);
	free(old_pwd_export_str);
	free(new_pwd_export_str);
	return (0);
}
 */
/* get_cd_path()
	Gets the target path for cd command.
	If no argument provided, uses HOME environment variable.
	if argument == '-', uses the OLDPWD environment variable.
*/
static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;

	path = args[1];
	if (!path)
	{
		path = get_env_value("HOME", shell);
		if (!path)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
			return (NULL);
		}
	}
	if (strncmp(path, "-", 2) == 0)
	{
		path = get_env_value("OLDPWD", shell);
		if (!path)
		{
			write(STDERR_FILENO, "minishell: cd: OLDPWD not set\n", 30);
			return (NULL);
		}
	}
	return (path);
}

/* update_pwd_vars()
	Updates OLDPWD and PWD environment variables.
*/
static int	update_pwd_vars(t_shell *shell, char *old_pwd_val, char *new_pwd)
{
	char	*old_pwd_export_str;
	char	*new_pwd_export_str;

	old_pwd_export_str = ft_strjoin("OLDPWD=", old_pwd_val);
	new_pwd_export_str = ft_strjoin("PWD=", new_pwd);
	if (!old_pwd_export_str || !new_pwd_export_str)
	{
		free(old_pwd_export_str);
		free(new_pwd_export_str);
		return (1);
	}
	set_env_var(shell, old_pwd_export_str);
	set_env_var(shell, new_pwd_export_str);
	free(old_pwd_export_str);
	free(new_pwd_export_str);
	return (0);
}

/* builtin_cd
	- if no argument: set path to $HOME value
	- if $HOME not defined: error, return 1
	- get old_pwd_val: Get the current PWD to save as OLDPWD later
	- Actually try to change the directory (if not ok, perror, return 1)
	- get the new PHYSICAL path using getcwd()
		- if error, perror, return 1 (error, but we did change directory.)
	- update OLDPWD and PWD, create OLDPWD=value, PWD=value
*/
int	builtin_cd(t_shell *shell, char **args)
{
	char	*path;
	char	*old_pwd_val;
	char	new_pwd_buffer[PATH_MAX];

	path = get_cd_path(args, shell);
	if (!path)
		return (1);
	if (args[1] && args[2] != NULL)
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	old_pwd_val = get_env_value("PWD", shell);
	if (chdir(path) != 0)
	{
		perror(path);
		return (1);
	}
	if (getcwd(new_pwd_buffer, sizeof(new_pwd_buffer)) == NULL)
	{
		perror("cd: getcwd error");
		return (1);
	}
	return (update_pwd_vars(shell, old_pwd_val, new_pwd_buffer));
}
