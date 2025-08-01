/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:39:16 by anemet            #+#    #+#             */
/*   Updated: 2025/06/05 10:57:11 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// The strlcpy() function copies up to size - 1 characters from
// the NUL-terminated string src to dst, NUL-terminating the result.

// 1.while loop copy src to dst, max size-1 bytes, last byte '\0'
// 2.while loop calculates and returns src length, regardless of truncation
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	if (size > 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[len] != '\0')
		len++;
	return (len);
}
