/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 16:55:55 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:47:06 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// helper to get the length of an unsigned number for ft_utoa
int	ft_num_len(unsigned int num)
{
	int	len;

	len = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num = num / 10;
		len++;
	}
	return (len);
}

// converts an unsigned int to a string
char	*ft_utoa(unsigned int n)
{
	char	*num;
	int		len;

	len = ft_num_len(n);
	num = (char *)malloc(sizeof(char) * (len + 1));
	if (!num)
		return (NULL);
	num[len] = '\0';
	len--;
	if (n == 0)
		num[0] = '0';
	while (n > 0)
	{
		num[len] = (n % 10 + '0');
		n = n / 10;
		len--;
	}
	return (num);
}
