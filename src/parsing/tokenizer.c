/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:09:07 by anemet            #+#    #+#             */
/*   Updated: 2025/08/01 17:17:28 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/*
  task : handle spaces and recognize metacharacters
  e.g.,: `ls|wc` should be ["ls", "|", "wc"], not ["ls|wc"]
*/

// A helper to check for metacharacters
// Returns the length of the metacharacter (1 or 2) or 0 if not metacharacter
int is_metachar(const char *s)
{
	if (*s == '|')
		return 1;
	if (*s == '<')
	{
		if (*(s+1) == '<')
			return 2;
		return 1;
	}
	if (*s == '>')
	{
		if (*(s+1) == '>')
			return 2;
		return 1;
	}
	return 0;
}

// for now a SIMPLIFIED tokenizer stub
/* --- The REAL tokenizer
  1. COUNT TOKENS:
    - loop through input
	- skip spaces
	- if metachar: increment token count and advance idx by 1 or 2
	- if regular character, this is the start of a word
	- increment token count and advance until space or metacharacter
  2. ALLOCATE MEMORY
    - char **tokens = malloc(sizeof(char *) * (token_count + 1));
	- NULL-terminate the array: tokens[token_count] = NULL;
  3. POPULATE ARRAY:
    - loop through input again
	- skip spaces
	- if metachar extract it (e.g., using ft_substr)
	- store it in the `tokens` array, and advance.
	- if it's a word, extract it, store it and advance
*/
char **tokenize(const char *input)
{
	char **tokens = malloc(sizeof(char *) * 4);
	if (!tokens)
		return NULL;
	tokens[0] = ft_strdup("ls");
	tokens[1] = ft_strdup("-l");
	tokens[2] = ft_strdup("|");
	tokens[3] = ft_strdup("wc");
	tokens[4] = ft_strdup("-l");
	tokens[4] = NULL;
	(void)input;
	return tokens;
}

// Free the given array of strings
void free_tokens(char **tokens)
{
	int i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

