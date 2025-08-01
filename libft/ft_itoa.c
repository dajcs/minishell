/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:35:15 by anemet            #+#    #+#             */
/*   Updated: 2025/06/11 10:12:51 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_res(char *res, long num, int len)
{
	int		i;
	char	c;

	if (num < 0)
	{
		num = -num;
		res[0] = '-';
	}
	if (num == 0)
		res[0] = '0';
	i = 0;
	while (num)
	{
		c = '0' + num % 10;
		res[len - 1 - i] = c;
		num = num / 10;
		i++;
	}
	res[len] = '\0';
	return ;
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;
	long	num;

	len = 0;
	if (n <= 0)
		len = 1;
	num = n;
	while (num)
	{
		len++;
		num = num / 10;
	}
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	fill_res(res, (long)n, len);
	return (res);
}
/*
#include <stdio.h>

int	main(int argc, char *argv[])
{
	char	*res;

	if (argc < 2)
	{
		printf("usage: %s <n>\n", argv[0]);
		return (1);
	}
	res = ft_itoa(atoi(argv[1]));
	printf("res = \"%s\"\n", res);
	return (0);
}
 */
