/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:34:10 by anemet            #+#    #+#             */
/*   Updated: 2025/08/02 20:12:38 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Placeholder for getting an environment variable's value
   - Use the real getenv() here
   - char *value = getenv(var_name);
   - return ft_strdup(value); // Return a copy
   - For now, a mock for testing for USER/HOME/HOST
   - Return empty string if not found, like bash
*/
static char *get_env_value(const char *var_name)
{
    if (ft_strcmp(var_name, "USER") == 0)
        return ft_strdup("anemet");
    if (ft_strcmp(var_name, "HOME") == 0)
        return ft_strdup("/home/anemet");
    if (ft_strcmp(var_name, "HOST") == 0)
        return ft_strdup("c2r7s5.42luxembourg.lu");
    return (ft_strdup(""));
}

/*
 1. Check the first character of the token.
    - If it's a single quote ('), do NOT expand. Just remove the outer quotes.
    - If it's a double quote ("), or no quote: expand variables
    - If starts with double quote, remove outer quotes.
 2. Variable expansion ($):
    - Scan the string for '$'.
    - When found, determine the variable name (e.g., "USER", "?", "HOME").
    - Get the value using getenv() or from last_exit_status for $?.
    - Reconstruct the string: [part before $] + [value] + [part after var].
    - Using ft_strjoin will be very helpful here.
    - Be careful: a new string is created, the old one must be freed.
 3. Quote removal:
    - After all expansion is done, if the token was quoted, create a new
      substring that omits the first and last characters.
*/
static char *process_single_token(char *token, int last_exit_status)
{
	int i;
	int j;
	char *var_name;
	char *var_value;

	if (token[0] == '\'')
		return ft_substr(token, 1, ft_strlen(token) - 2);
	while (token[i])
	{
		while (token[i] && token[i] != '$')
			i++;
		i++;
		j = 0;
		while (token[i] && !is_space(token[i]))
			j++;
		var_name = ft_substr(token, i, j);
		var_value = get_env_value(var_name);
	}
}
