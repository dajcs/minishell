/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:07:11 by anemet            #+#    #+#             */
/*   Updated: 2025/08/10 16:43:08 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* builtin_pwd()
	- (void)args; // pwd takes no options
	- get "PWD" value from envp
	- if found print, return
	- otherwise: fallback case getcwd()
	- if that fails perror, return 1
*/
int	builtin_pwd(t_shell *shell, char **args)
{
	char	*pwd_val;
	char	cwd_buffer[PATH_MAX];

	(void)args;
	pwd_val = get_env_value("PWD", shell);
	if (pwd_val)
	{
		printf("%s\n", pwd_val);
		free(pwd_val);
		return (0);
	}
	else
	{
		if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
		{
			printf("%s\n", cwd_buffer);
			return (0);
		}
		perror("pwd");
		return (1);
	}
}
