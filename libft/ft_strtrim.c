/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 12:13:22 by anemet            #+#    #+#             */
/*   Updated: 2025/06/10 13:32:16 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*out;
	size_t	start;
	size_t	end;
	size_t	len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, (unsigned char)s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, (unsigned char)s1[end - 1]))
		end--;
	len = end - start;
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, s1 + start, len);
	out[len] = '\0';
	return (out);
}
