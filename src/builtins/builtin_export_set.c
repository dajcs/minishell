/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_set.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:30:30 by anemet            #+#    #+#             */
/*   Updated: 2025/08/06 17:41:06 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_new_var(t_shell *shell, char *var_assignment)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (shell->envp_list[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return ;
	i = 0;
	while (i < count)
	{
		new_envp[i] = shell->envp_list[i];
		i++;
	}
	new_envp[count] = ft_strdup(var_assignment);
	new_envp[count + 1] = NULL;
	free(shell->envp_list);
	shell->envp_list = new_envp;
}

static void	update_existing_var(t_shell *shell, int index, char *new_var)
{
	char	*new_assignment;

	new_assignment = ft_strdup(new_var);
	if (!new_assignment)
		return ;
	free(shell->envp_list[index]);
	shell->envp_list[index] = new_assignment;
}

int	find_var_index(char **envp, char *var_name)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return i;
		i++;
	}
	return (-1);
}

void	set_env_var(t_shell *shell, char *var_assignment)
{
	char	*equals_pos;
	char	*var_name;
	int		existing_index;

	equals_pos = ft_strchr(var_assignment, '=');
	if (!equals_pos)
		return ;
	var_name = ft_substr(var_assignment, 0, equals_pos - var_assignment);
	if (!var_name)
		return ;
	existing_index = find_var_index(shell->envp_list, var_name);
	if (existing_index >= 0)
		update_existing_var(shell, existing_index, var_assignment);
	else
		add_new_var(shell, var_assignment);
	free(var_name);
}
