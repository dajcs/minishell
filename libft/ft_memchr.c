/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 13:18:23 by anemet            #+#    #+#             */
/*   Updated: 2025/06/05 14:08:55 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// memchr() scans the initial n bytes of the memory area pointed to by s
// for the first instance of c (interpreted as unsigned char)
// return a pointer to the matching byte or NULL
// void *s pointer copied to unsigned char *ptr so we walk up in 1 byte steps
// return type must be casted to (void *) because of prototype
void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	size_t				i;

	i = 0;
	ptr = (const unsigned char *)s;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
