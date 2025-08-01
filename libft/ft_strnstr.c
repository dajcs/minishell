/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:09:42 by anemet            #+#    #+#             */
/*   Updated: 2025/06/05 14:22:20 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// strnstr() locates the first occurrence of the null-terminated string
// little in the string big, where not more than len characters are searched.
// 1. measure the needle (little) -> get length in `l_len`
// 2. if empty needle (l_len == 0) -> instant hit, return `big`
// 3. scan haystack (big) within `len`
//    - loop over big while there is room for needle (i + l_len <= len)
//    - at each position `i` compare the next `l_len` bytes
//    - on full match return pointer on that spot (big + i)
// 4. No match -> NULL.
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	l_len;

	i = 0;
	j = 0;
	l_len = 0;
	while (little[l_len] != '\0')
		l_len++;
	if (l_len == 0)
		return ((char *)big);
	while (big[i] != '\0' && i + l_len <= len)
	{
		j = 0;
		while (j < l_len && big[i + j] == little[j])
			j++;
		if (j == l_len)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
