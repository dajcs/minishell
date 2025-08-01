/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:13:46 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:46:47 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// recursively prints a pointer address
void	ft_put_ptr(uintptr_t num)
{
	if (num < 16)
	{
		if (num < 10)
			ft_putchar_fd('0' + num, 1);
		else
			ft_putchar_fd('a' + (num - 10), 1);
	}
	else
	{
		ft_put_ptr(num / 16);
		ft_put_ptr(num % 16);
	}
}

// calculates the length of a pointer's hex representation
int	ft_ptr_len(uintptr_t num)
{
	int	len;

	len = 0;
	while (num != 0)
	{
		len++;
		num = num / 16;
	}
	return (len);
}

// reverse string s, trust in len, there is no '\0'
void	ft_strrev(char *s, int len)
{
	int		start;
	int		end;
	char	temp;

	start = 0;
	end = len - 1;
	while (start < end)
	{
		temp = s[start];
		s[start] = s[end];
		s[end] = temp;
		start++;
		end--;
	}
}
