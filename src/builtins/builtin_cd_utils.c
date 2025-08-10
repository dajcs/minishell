/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:53:39 by anemet            #+#    #+#             */
/*   Updated: 2025/08/10 16:54:48 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* expand_tilde_path()
	Expands leading ~ with HOME environment variable.
	Returns 0 on success, 1 on error.
*/
int	expand_tilde_path(char *dest, const char *src, t_shell *shell)
{
	char	*home_val;

	if (src[0] != '~')
	{
		ft_strlcpy(dest, src, PATH_MAX);
		return (0);
	}
	home_val = get_env_value("HOME", shell);
	if (!home_val)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		return (1);
	}
	ft_strlcpy(dest, home_val, PATH_MAX);
	ft_strlcat(dest, src + 1, PATH_MAX);
	free(home_val);
	return (0);
}

/* handle_home_path()
	Handles cd with no arguments (HOME directory).
*/
int	handle_home_path(char *dest, t_shell *shell)
{
	char	*env_val;

	env_val = get_env_value("HOME", shell);
	if (!env_val)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		return (1);
	}
	ft_strlcpy(dest, env_val, PATH_MAX);
	free(env_val);
	return (0);
}
