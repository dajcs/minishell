/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_cs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 11:14:27 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:46:26 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ft_print_cs.c -> print character and string */

#include "libft.h"

int	ft_print_char(char c, t_flags flags)
{
	int	count;

	count = 0;
	if (flags.minus == 1)
		count += write(1, &c, 1);
	count += ft_print_padding(' ', flags.width - 1);
	if (flags.minus == 0)
		count += write(1, &c, 1);
	return (count);
}

/*
** The logic for handling NULL pointers with %s is tricky.
** A standard printf often truncates "(null)" (e.g., "%.3s" -> "(nu".
** The printf on our system prints an empty string if precision < 6.
*/

int	ft_print_str(char *str, t_flags flags)
{
	int	count;
	int	len;

	count = 0;
	if (!str)
	{
		if (flags.dot && flags.precision < 6)
			str = "";
		else
			str = "(null)";
	}
	len = ft_strlen(str);
	if (flags.dot && flags.precision < len)
		len = flags.precision;
	if (flags.minus == 1)
		count += write(1, str, len);
	count += ft_print_padding(' ', flags.width - len);
	if (flags.minus == 0)
		count += write(1, str, len);
	return (count);
}

// a general-purpose padding utility
int	ft_print_padding(char c, int len)
{
	int	count;

	count = 0;
	while (len > 0)
	{
		write(1, &c, 1);
		count++;
		len--;
	}
	return (count);
}
