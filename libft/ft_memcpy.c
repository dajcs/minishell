/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:03:44 by anemet            #+#    #+#             */
/*   Updated: 2025/06/06 13:00:13 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// d = (unsigned char *)dest because (void *)dest can't be set
// size_t is an unsigned int considering the hw/sw architecture (eg 32/64 bit)
// s = (const unsigned char *)src - const makes sure to not modify src content
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
