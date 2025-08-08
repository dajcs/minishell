/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:34:10 by anemet            #+#    #+#             */
/*   Updated: 2025/08/08 16:09:55 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* get_env_value
	- Using getenv() to get value
	- char *value = getenv(var_name);
	- return (ft_strdup(value)); // Return a copy
	- Return empty string if not found, like bash
*/
char	*get_env_value(const char *var_name, t_shell *shell_data)
{
	char	**envp;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var_name);
	envp = shell_data->envp_list;
	if (!envp || !var_name)
		return (ft_strdup(""));
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

/* handle_variable_expansion()
   Helper function to handle the expansion of a variable.
	- It extracts the variable name,
		gets its value, and appends it to the new token
*/
static void	handle_variable_expansion(char **new_token, int *i, char *token,
		t_shell *shell_data)
{
	int		j;
	char	*var_name;
	char	*var_value;

	j = *i + 1;
	if (token[j] == '?')
		j++;
	else
		while (ft_isalnum(token[j]) || token[j] == '_')
			j++;
	var_name = ft_substr(token, *i + 1, j - (*i + 1));
	if (ft_strncmp(var_name, "?", 2) == 0)
		var_value = ft_itoa(shell_data->last_exit_status);
	else
		var_value = get_env_value(var_name, shell_data);
	free(var_name);
	if (var_value)
	{
		*new_token = ft_strjoin(*new_token, var_value);
		free(var_value);
	}
	*i = j - 1;
}

/* process_token_chars()
   Processes the token character by character to build the new token.
	- It handles single quotes, double quotes, and variable expansion.
*/
static void	process_token_chars(char *token, char **new_token,
		t_shell *shell_data)
{
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (token[i])
	{
		if ((token[i] == '\'' || token[i] == '"') && !in_quote)
			in_quote = token[i];
		else if (token[i] == in_quote)
			in_quote = 0;
		else if (token[i] == '$' && in_quote != '\'')
		{
			if (!token[i + 1] || is_space(token[i + 1]) || token[i + 1] == '"')
				*new_token = ft_strjoin_char(*new_token, token[i]);
			else
				handle_variable_expansion(new_token, &i, token, shell_data);
		}
		else
			*new_token = ft_strjoin_char(*new_token, token[i]);
		i++;
	}
}

/* process_single_token()
	Replicates bash variable expansion and quote removal for a single token.
	- Expands variables inside double quotes or unquoted.
	- Does not expand variables inside single quotes.
	- Outer quote rules apply for nested quotes.
	- Removes outer quotes after variable expansion.
	- Leaves '$' followed by a space or '"' as is.
*/
static char	*process_single_token(char *token, t_shell *shell_data)
{
	char	*new_token;

	new_token = ft_strdup("");
	if (!new_token)
		return (NULL);
	process_token_chars(token, &new_token, shell_data);
	return (new_token);
}

// expands variables in tokens and removes outer quotes
// returns a new token list **new_tokens
char	**expand_and_clean(char **tokens, t_shell *shell_data)
{
	char	**new_tokens;
	int		i;
	int		count;

	count = 0;
	while (tokens[count])
		count++;
	new_tokens = malloc(sizeof(char *) * (count + 1));
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		new_tokens[i] = process_single_token(tokens[i], shell_data);
		i++;
	}
	new_tokens[i] = NULL;
	return (new_tokens);
}
