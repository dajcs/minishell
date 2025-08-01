/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:34:31 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:46:17 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// initialize the flags struct to a default state
t_flags	ft_init_flags(void)
{
	t_flags	flags;

	flags.minus = 0;
	flags.zero = 0;
	flags.width = 0;
	flags.dot = 0;
	flags.precision = 0;
	flags.hash = 0;
	flags.plus = 0;
	flags.space = 0;
	return (flags);
}

static void	ft_parse_flags(t_flags *flags, const char *format, int *i)
{
	while (ft_strchr("-0#+ ", format[*i]))
	{
		if (format[*i] == '-')
			flags->minus = 1;
		if (format[*i] == '0')
			flags->zero = 1;
		if (format[*i] == '#')
			flags->hash = 1;
		if (format[*i] == '+')
			flags->plus = 1;
		if (format[*i] == ' ')
			flags->space = 1;
		(*i)++;
	}
}

// parses flags, width and precision from the format string in order:
// (minus | zero | hash | plus | space) (width) (dot + precision)
void	ft_parse_flags_width_prec(t_flags *flags, const char *format, int *i)
{
	ft_parse_flags(flags, format, i);
	if (ft_isdigit(format[*i]))
	{
		flags->width = ft_atoi(&format[*i]);
		while (ft_isdigit(format[*i]))
			(*i)++;
	}
	if (format[*i] == '.')
	{
		flags->dot = 1;
		(*i)++;
		flags->precision = ft_atoi(&format[*i]);
		while (ft_isdigit(format[*i]))
			(*i)++;
	}
}

// dispatches the correct printing function based on the type
int	ft_dispatch(char type, t_flags flags, va_list args)
{
	int	count;

	count = 0;
	if (type == 'c')
		count = ft_print_char(va_arg(args, int), flags);
	else if (type == 's')
		count = ft_print_str(va_arg(args, char *), flags);
	else if (type == 'd' || type == 'i')
		count = ft_print_nbr(va_arg(args, int), flags);
	else if (type == 'u')
		count = ft_print_unsigned(va_arg(args, unsigned int), flags);
	else if (type == 'x' || type == 'X')
		count = ft_print_hex(va_arg(args, unsigned int), flags, type);
	else if (type == 'p')
		count = ft_print_ptr(va_arg(args, uintptr_t), flags);
	else if (type == '%')
		count = ft_print_char('%', flags);
	return (count);
}
