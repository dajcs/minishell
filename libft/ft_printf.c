/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 07:44:35 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:46:56 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*

bonus printf format structure: %[flags][width][.precision][conversion type]

- flags:
  - `-` minus - left align output
  - `0` zero - pad numbers with 0-s instead of spaces
  - `#` hash - prefix `0x` or `0X` for hex
  - `+` plus - forces a `+` before positive numeric output
  - ` ` space - inserts a ` ` space before positive numbers
- width:
  - sets the minimum number of characters to be printed.
    if content is shorter it will be padded with spaces (or zeros if flagged)
- precision:
  - sets the maximum number of characters for strings
  - sets the minimum number of digits for int (padded with zeros if needed)
  - sets the number of digis after decimal point for floats (info only)
- conversion type:
  - `c` Prints a single character.
  - `s` Prints a string (as defined by the common C convention).
  - `p` The void * pointer argument has to be printed in hexadecimal format.
  - `d` Prints a decimal (base 10) number.
  - `i` Prints an integer in base 10.
  - `u` Prints an unsigned decimal (base 10) number.
  - `x` Prints a number in hexadecimal (base 16) lowercase format.
  - `X` Prints a number in hexadecimal (base 16) uppercase format.
  - `%` Prints a percent sign.
*/

// the anchor function. Iterates over the format string
// va_start(args, format)  points 'args' to the first argument after 'format'
// va_end(args) closes the variadic "file handle"
int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	args;
	int		total_len;

	i = 0;
	total_len = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
			total_len += ft_eval_format(args, format, &i);
		else
		{
			ft_putchar_fd(format[i], 1);
			total_len++;
		}
		i++;
	}
	va_end(args);
	return (total_len);
}

// evaluates the complete format specifier (flags, width, etc.)
// and calls the dispatcher. The index i is passed by reference
// to advance the main loop's counter past the format specifier
int	ft_eval_format(va_list args, const char *format, int *i)
{
	t_flags	flags;
	char	type;

	(*i)++;
	flags = ft_init_flags();
	ft_parse_flags_width_prec(&flags, format, i);
	type = format[*i];
	if (ft_strchr("cspdiuxX%", type))
		return (ft_dispatch(type, flags, args));
	return (0);
}
