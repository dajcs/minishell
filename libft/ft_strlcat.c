/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:09:14 by anemet            #+#    #+#             */
/*   Updated: 2025/06/05 11:18:14 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// The strlcat() function appends the NUL-terminated string src to
// the end of dst. It will append at most size - strlen(dst) - 1 bytes,
//                                              NUL-terminating the result.

// d walks dst (but never past size) to get dst length
// s walks src to get src length
// if dst already full (d >= size), bail early, return whisful (size + s)
// append chars from src to dst until end of src or 1 byte left
// NUL-terminate and return len(src) + len(dst)
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d;
	size_t	s;
	size_t	i;

	d = 0;
	s = 0;
	i = 0;
	while (dst[d] != '\0' && d < size)
		d++;
	while (src[s] != '\0')
		s++;
	if (d >= size)
		return (size + s);
	while (src[i] != '\0' && d + i < size - 1)
	{
		dst[d + i] = src[i];
		i++;
	}
	dst[d + i] = '\0';
	return (d + s);
}
