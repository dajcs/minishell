/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:02:14 by anemet            #+#    #+#             */
/*   Updated: 2025/06/10 15:57:13 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ft_split
** Splits string 's' on delimiter 'c' and return a NULL-terminated array
**                                                                of substrings.
** Returns NULL if any allocation fails.
*/

#include "libft.h"

// count the words separated by c
// outer while loop
// 		1. skip initial/consecutive separators while increasing i
// 		2. if s[i] != c increase count
// 		3. 		increase i until next next c
static size_t	ft_count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

// free partially-filled split array on error
static char	**ft_free_split(char **tab, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

// fill the array with substrings; on failure return NULL
static char	**ft_fill_split(char **tab, char const *s, char c, size_t wc)
{
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	j = 0;
	while (j < wc)
	{
		while (s[i] == c)
			i++;
		len = 0;
		while (s[i + len] && s[i + len] != c)
			len++;
		tab[j] = ft_substr(s, i, len);
		if (!tab[j])
			return (ft_free_split(tab, j));
		i += len;
		j++;
	}
	tab[j] = NULL;
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	wc;

	if (!s)
		return (NULL);
	wc = ft_count_words(s, c);
	tab = (char **)malloc((wc + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	return (ft_fill_split(tab, s, c, wc));
}
