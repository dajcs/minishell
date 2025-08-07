/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:47:51 by anemet            #+#    #+#             */
/*   Updated: 2025/08/07 19:07:34 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	bubble_sort_env(char **env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_env(char **env)
{
	int		i;
	char	*equals_pos;
	char	*var_name;
	char	*var_value;

	i = 0;
	while (env[i])
	{
		equals_pos = ft_strchr(env[i], '=');
		if (equals_pos)
		{
			var_name = ft_substr(env[i], 0, equals_pos - env[i]);
			var_value = equals_pos + 1;
			printf("declare -x %s=\"%s\"\n", var_name, var_value);
			free(var_name);
		}
		i++;
	}
}

static void	print_exported_vars(char **envp)
{
	char	**sorted_env;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return ;
	i = 0;
	while (i < count)
	{
		sorted_env[i] = ft_strdup(envp[i]);
		i++;
	}
	sorted_env[count] = NULL;
	bubble_sort_env(sorted_env, count);
	print_sorted_env(sorted_env);
	free_tokens(sorted_env);
}

static int	export_error(const char *identifier)
{
	write(STDERR_FILENO, "minishell: export: `", 20);
	write(STDERR_FILENO, identifier, ft_strlen(identifier));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	return (1);
}

int	builtin_export(t_shell *shell, char **args)
{
	int		i;
	char	*var_name;

	if (args[1] == NULL)
	{
		print_exported_vars(shell->envp_list);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		var_name = get_var_name(args[i]);
		if (!var_name || !is_valid_identifier(var_name))
		{
			free(var_name);
			return (export_error(args[i]));
		}
		else if (ft_strchr(args[i], '='))
			set_env_var(shell, args[i]);
		free(var_name);
		i++;
	}
	return (0);
}
