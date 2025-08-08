/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:40:42 by anemet            #+#    #+#             */
/*   Updated: 2025/08/08 16:08:17 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(len1 + len2 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	return (str);
}

/* ft_strjoin_char()
	Appends a character 'c' to the end of the string 's1'.
	- Allocates a new string for the result, copies 's1'
		into it, adds 'c' at the end, and null-terminates it.
	- It then frees the original string 's1' to prevent memory leaks
		in iterative string-building scenarios.
	- Return: a new, null-terminated string containing the result
*/
char	*ft_strjoin_char(char *s1, char c)
{
	char	*new_str;
	int		s1_len;
	int		i;

	if (!s1)
		s1_len = 0;
	else
		s1_len = ft_strlen(s1);
	new_str = (char *)malloc(sizeof(char) * (s1_len + 2));
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	i = 0;
	while (i < s1_len)
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i] = c;
	i++;
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}
