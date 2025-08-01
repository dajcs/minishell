/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_px.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:33:10 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:46:43 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
ft_print_px.c -> print pointer & hexadecimal
similar logic to integer printing, but accounting for the 0x prefix pointers
*/

#include "libft.h"

// calculate padding for ft_out_hex
// pads[0] = zero_pad, pads[1] = space_pad
static void	ft_get_hex_pads(t_flags f, int len, int p_len, int *pads)
{
	pads[0] = f.precision - len;
	if (pads[0] < 0)
		pads[0] = 0;
	if (f.zero)
	{
		pads[0] = f.width - len - p_len;
		if (pads[0] < 0)
			pads[0] = 0;
	}
	pads[1] = f.width - len - pads[0] - p_len;
	if (pads[1] < 0)
		pads[1] = 0;
}

// handles the assembly of the final hex output
// pads[2]: pads[0] = zero_pad; pads[1] = space_pad
static int	ft_out_hex(char *s, t_flags f, int len, char *prefix)
{
	int	count;
	int	pads[2];
	int	prefix_len;

	count = 0;
	if (f.minus || f.dot)
		f.zero = 0;
	prefix_len = ft_strlen(prefix);
	ft_get_hex_pads(f, len, prefix_len, pads);
	if (f.minus == 0)
		count += ft_print_padding(' ', pads[1]);
	count += write(1, prefix, prefix_len);
	count += ft_print_padding('0', pads[0]);
	count += write(1, s, len);
	if (f.minus == 1)
		count += ft_print_padding(' ', pads[1]);
	return (count);
}

// Helper: Converts unsigned int to a hex string in buffer 's'.
static int	ft_uitohex_str(unsigned int n, char *s, char *hex_chars)
{
	int	i;

	i = 0;
	if (n == 0)
		s[i++] = '0';
	while (n > 0)
	{
		s[i++] = hex_chars[n % 16];
		n /= 16;
	}
	ft_strrev(s, i);
	return (i);
}

int	ft_print_hex(unsigned int n, t_flags flags, char format)
{
	char	s[12];
	char	*hex_chars;
	char	*prefix;
	int		len;

	if (flags.dot && flags.precision == 0 && n == 0)
		return (ft_print_padding(' ', flags.width));
	prefix = "";
	if (flags.hash && n != 0)
	{
		if (format == 'x')
			prefix = "0x";
		else
			prefix = "0X";
	}
	if (format == 'x')
		hex_chars = "0123456789abcdef";
	else
		hex_chars = "0123456789ABCDEF";
	len = ft_uitohex_str(n, s, hex_chars);
	return (ft_out_hex(s, flags, len, prefix));
}

// Simplified pointer printing logic
// For simplicity, we'll do a basic padded version
// len = ft_ptr_len(ptr) + 2; # +2 for "0x"
// if ptr is NULL the precision flag is ignored
int	ft_print_ptr(uintptr_t ptr, t_flags flags)
{
	int	count;
	int	len;

	count = 0;
	if (ptr == 0)
	{
		len = 5;
		if (flags.minus == 0)
			count += ft_print_padding(' ', flags.width - len);
		count += write(1, "(nil)", len);
		if (flags.minus == 1)
			count += ft_print_padding(' ', flags.width - len);
		return (count);
	}
	len = ft_ptr_len(ptr) + 2;
	if (flags.minus == 0)
		count += ft_print_padding(' ', flags.width - len);
	count += write(1, "0x", 2);
	ft_put_ptr(ptr);
	count += len - 2;
	if (flags.minus == 1)
		count += ft_print_padding(' ', flags.width - len);
	return (count);
}
