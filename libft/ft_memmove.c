/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:11:31 by anemet            #+#    #+#             */
/*   Updated: 2025/06/06 13:30:03 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// d = (unsigned char *)dest because (void *)dest can't be set
// size_t is an unsigned int considering the hw/sw architecture (eg 32/64 bit)
// s = (const unsigned char *)src - const makes sure to not modify src content
// d < s copy forward
// d > s copy backwards so overlapping ranges don't clobber data
// if overlapping areas, after memmove *src might be modified despite const
// return (dest) unconditionally - even if d == s
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	if (d < s)
	{
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	if (d > s)
	{
		while (n > 0)
		{
			n--;
			d[n] = s[n];
		}
	}
	return (dest);
}
