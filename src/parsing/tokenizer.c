/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:09:07 by anemet            #+#    #+#             */
/*   Updated: 2025/08/04 17:56:50 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
  task : handle spaces and recognize metacharacters and quotations
  e.g.,: `ls|wc` should be ["ls", "|", "wc"], not ["ls|wc"]
*/

/* count_tokens:
	- outer while loop iterates through input `s` string
	- skip spaces
	- if something: increment token count
	- if `quote_char`: advance i by length of quote; outer while next iteration
	- if !(quote_char)
		- if `metachar`: advance i by 1 or 2; outer while next iteration
		- if !(metachar): advance until next space/meta/quote char
	- return count
*/
static int	count_tokens(const char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s && s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (s[i])
			count++;
		if (!check(&is_quotechar, s, &i))
		{
			if (!check(&is_metachar, s, &i))
			while (s[i] && !is_space(s[i]) && !is_metachar(s + i)
				&& !is_quotechar(s + i))
				i++;
		}
	}
	return (count);
}

/* extract_tokens:
	- skip spaces
	- if quotechar extract it using ft_substr, advance *i
	- if metachar extract it using ft_substr, advance *i
	- if it's a word, extract it, store it and advance *i
	- if storage OK, return token, otherwise return NULL
*/
static char	*extract_tokens(const char *s, int *i)
{
	int		j;
	char	*token;

	while (s[*i] && is_space(s[*i]))
		(*i)++;
	token = extract_if(s, i, &is_quotechar);
	if (!token)
	{
		token = extract_if(s, i, &is_metachar);
		if (!token)
		{
			j = 0;
			while (s[*i + j] && !is_space(s[*i + j]) && !is_metachar(s + *i + j)
				&& !is_quotechar(s + *i + j))
				j++;
			token = ft_substr(s, *i, j);
			*i += j;
		}
	}
	return (token);
}

/*   Tokenizer steps:
  1. count_tokens(input)
  2. allocate memory for char **tokens: nr_tokens + 1 for terminating NULL
  3. populate array: *extract_tokens
  4. Return **tokens;
*/
char	**tokenize(const char *input)
{
	int		i;
	int		tok;
	int		nr_tokens;
	char	**tokens;

	nr_tokens = count_tokens(input);
	tokens = malloc(sizeof(char *) * (nr_tokens + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	tok = 0;
	while (tok < nr_tokens)
	{
		tokens[tok] = extract_tokens(input, &i);
		if (!tokens[tok])
		{
			free_tokens(tokens);
			return (NULL);
		}
		tok++;
	}
	tokens[tok] = NULL;
	return (tokens);
}

// Free the given array of strings
void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
