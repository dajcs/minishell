/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:48:28 by anemet            #+#    #+#             */
/*   Updated: 2025/06/05 15:00:33 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			total;
	size_t			i;
	unsigned char	*ptr;

	total = 0;
	i = 0;
	ptr = NULL;
	if (nmemb == 0 || size == 0)
		total = 1;
	else if (nmemb > SIZE_MAX / size)
		return (NULL);
	else
		total = nmemb * size;
	ptr = (unsigned char *)malloc(total);
	if (ptr == NULL)
		return (NULL);
	while (i < total)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}
