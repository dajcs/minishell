/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:09:07 by anemet            #+#    #+#             */
/*   Updated: 2025/08/02 11:20:13 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/*
  task : handle spaces and recognize metacharacters
  e.g.,: `ls|wc` should be ["ls", "|", "wc"], not ["ls|wc"]
*/

// A helper to check for metacharacters
// Returns the length of the metacharacter (1 or 2) or 0 if not metacharacter
static int	is_metachar(const char *s)
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

/* COUNT TOKENS:
	- loop through input
	- skip spaces
	- if something: increment token count
	- if metachar: advance i by 1 or 2
	- if regular character: advance until space or metacharacter
	- return count                                                  */
static int	count_tokens(const char *s)
{
	int	i;
	int	meta;
	int	count;

	i = 0;
	count = 0;
	while (s && s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (s[i])
			count++;
		meta = is_metachar(s + i);
		if (meta)
			i += meta;
		else
			while (s[i] && !is_space(s[i]) && !is_metachar(s + i))
				i++;
	}
	return (count);
}

/* Extract Tokens:
	- skip spaces
	- if metachar extract it using ft_substr), advance *i
	- if it's a word, extract it, store it and advance *i
	- if storage OK, return token, otherwise return NULL       */
static char	*extract_tokens(const char *s, int *i)
{
	int		j;
	int		meta;
	char	*token;

	while (s[*i] && is_space(s[*i]))
		(*i)++;
	meta = is_metachar(s + *i);
	if (meta)
	{
		token = ft_substr(s, *i, meta);
		*i += meta;
	}
	else
	{
		j = 0;
		while (s[*i + j] && !is_space(s[*i + j]) && !is_metachar(s + *i + j))
			j++;
		token = ft_substr(s, *i, j);
		*i += j;
	}
	if (!token)
		return (NULL);
	return (token);
}

/*   Tokenizer steps:
  1. count_tokens(input)
  2. allocate memory for char **tokens: nr_tokens + 1 for terminating NULL
  3. populate array: *extract_tokens
  4. Return **tokens;                                                   */
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
			return (NULL);
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
