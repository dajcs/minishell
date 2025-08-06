/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:51:44 by anemet            #+#    #+#             */
/*   Updated: 2025/08/06 17:57:13 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void remove_var_at_index(t_shell *shell, int var_index)
{
	int j;

	free(shell->envp_list[var_index]);
	j = var_index;
	while (shell->envp_list[j + 1])
	{
		shell->envp_list[j] = shell->envp_list[j + 1];
		j++;
	}
	shell->envp_list[j] = NULL;
}

int builtin_unset(t_shell *shell, char **args)
{
	int i;
	int var_index;

	i = 1;
	while (args[i])
	{
		var_index = find_var_index(shell->envp_list, args[i]);
		if (var_index != -1)
			remove_var_at_index(shell, var_index);
		i++;
	}
	return 0;
}
