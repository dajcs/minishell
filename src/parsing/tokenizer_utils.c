/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:13:22 by anemet            #+#    #+#             */
/*   Updated: 2025/08/03 16:55:53 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A helper to check for metacharacters
// Returns the length of the metacharacter (1 or 2) or 0 if not metacharacter
int	is_metachar(const char *s)
{
	if (*s == '|')
		return (1);
	if (*s == '<')
	{
		if (*(s + 1) == '<')
			return (2);
		return (1);
	}
	if (*s == '>')
	{
		if (*(s + 1) == '>')
			return (2);
		return (1);
	}
	return (0);
}

// A helper to check for quote_char ' or "
// Returns the length of the string enclosed in quotes
// if not quote_char, return 0
int	is_quotechar(const char *s)
{
	int		i;
	char	quote_char;

	i = 0;
	if (*s == '\'' || *s == '"')
	{
		quote_char = s[0];
		i++;
		while (s[i] && s[i] != quote_char)
			i++;
		if (s[i])
			i++;
	}
	return (i);
}

/* check():
	- call checker_func(s + *i)
	- if it matches, advance *i by returned length
	- return the length advanced (or 0 if no match)
*/
int	check(int (*checker_func)(const char *), const char *s, int *i)
{
	int	len;

	len = checker_func(s + *i);
	if (len)
		*i += len;
	return (len);
}

/* extract_if
	- call checker_func(s + *i)
	- if it matches, ft_substr and advance *i by returned length
	- return the extracted token pointer or NULL
*/
char	*extract_if(const char *s, int *i, int (*checker_func)(const char *))
{
	int		len;
	char	*token;

	token = NULL;
	len = checker_func(s + *i);
	if (len)
	{
		token = ft_substr(s, *i, len);
		*i += len;
	}
	return (token);
}

/* end_varchar
	- advances *i through the string s over "ordinary" characters
	- stops when encounters space/$/meta/quote character
*/
void	end_varchar(const char *s, int *i)
{
	if (s[*i] == '?')
	{
		(*i)++;
		return ;
	}
	while (s[*i] && !is_space(s[*i]) && s[*i] != '$'
		&& !is_metachar(s + *i) && !is_quotechar(s + *i))
		(*i)++;
}
