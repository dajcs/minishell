/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 08:14:03 by anemet            #+#    #+#             */
/*   Updated: 2025/08/10 16:54:06 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* handle_oldpwd_path()
	Handles cd with '-' argument (OLDPWD directory).
*/
static int	handle_oldpwd_path(char *dest, t_shell *shell)
{
	char	*env_val;

	env_val = get_env_value("OLDPWD", shell);
	if (!env_val)
	{
		write(STDERR_FILENO, "minishell: cd: OLDPWD not set\n", 30);
		return (1);
	}
	ft_strlcpy(dest, env_val, PATH_MAX);
	free(env_val);
	return (0);
}

/* get_cd_path()
	Gets the target path for cd command and handles special cases.
	Returns 0 on success, 1 on error.
*/
static int	get_cd_path(char *dest, char **args, t_shell *shell)
{
	if (!args[1])
		return (handle_home_path(dest, shell));
	if (ft_strncmp(args[1], "-", 2) == 0)
		return (handle_oldpwd_path(dest, shell));
	return (expand_tilde_path(dest, args[1], shell));
}

/* update_pwd_vars()
	Updates OLDPWD and PWD environment variables.
*/
static int	update_pwd_vars(t_shell *shell, const char *old_pwd,
		const char *new_pwd)
{
	char	old_pwd_str[PATH_MAX + 8];
	char	new_pwd_str[PATH_MAX + 5];

	ft_strlcpy(old_pwd_str, "OLDPWD=", sizeof(old_pwd_str));
	ft_strlcat(old_pwd_str, old_pwd, sizeof(old_pwd_str));
	ft_strlcpy(new_pwd_str, "PWD=", sizeof(new_pwd_str));
	ft_strlcat(new_pwd_str, new_pwd, sizeof(new_pwd_str));
	set_env_var(shell, old_pwd_str);
	set_env_var(shell, new_pwd_str);
	return (0);
}

/* prepare_old_pwd()
	Prepares the old PWD value for later use.
*/
static void	prepare_old_pwd(char *old_pwd_buffer, t_shell *shell)
{
	char	*old_pwd_val;

	old_pwd_val = get_env_value("PWD", shell);
	if (old_pwd_val)
	{
		ft_strlcpy(old_pwd_buffer, old_pwd_val, PATH_MAX);
		free(old_pwd_val);
	}
	else
		old_pwd_buffer[0] = '\0';
}

/* builtin_cd
	Changes directory with support for HOME, OLDPWD, and tilde expansion.
	Updates PWD and OLDPWD environment variables.
*/
int	builtin_cd(t_shell *shell, char **args)
{
	char	path[PATH_MAX];
	char	old_pwd_buffer[PATH_MAX];
	char	new_pwd_buffer[PATH_MAX];

	if (args[1] && args[2])
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (get_cd_path(path, args, shell) != 0)
		return (1);
	prepare_old_pwd(old_pwd_buffer, shell);
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
	return (update_pwd_vars(shell, old_pwd_buffer, new_pwd_buffer));
}
