/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:01:58 by anemet            #+#    #+#             */
/*   Updated: 2025/08/07 19:11:06 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* get_var_name()
	Extracts the variable name from an "NAME=value" string.
	Returns a newly allocated string with the name.
*/
char	*get_var_name(const char *assignment)
{
	char	*equal_sign;
	int		len;

	equal_sign = ft_strchr(assignment, '=');
	if (equal_sign)
		len = equal_sign - assignment;
	else
		len = ft_strlen(assignment);
	return (ft_substr(assignment, 0, len));
}

/* is_valid_identifier()
	Checks if a string is a valid shell identifier.
	Returns 1 if valid, 0 otherwise.
	- Rule 0: Must not be NULL or empty.
	- Rule 1: First character must be a letter or underscore.
	- Rule 2: Subsequent characters must be alphanumeric or underscore.
*/
int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
