/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:26:28 by anemet            #+#    #+#             */
/*   Updated: 2025/06/05 14:37:12 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// 1. whitespace skip - advancing `i` on ' ' or ASCII 9..13 tabs
// 2. sign parsing - advancing `i` and adjusting `sign` when leading '-'
// 3. digit loop - accumulating value in `result` as `long` to avoid overflow
// 4. return result casted to (int)
int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return ((int)(result * sign));
}
