/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_diu.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 11:35:26 by anemet            #+#    #+#             */
/*   Updated: 2025/07/03 11:46:38 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
ft_print_diu.c -> print int & unsigned int.
interactions between `0`, `-`, `#`, `+`, ` `, `width`, `.`, `precision`and
negative numbers
*/

#include "libft.h"

// Helper 1: Calculate all padding and prefix lengths.
// Returns the final length of the number string to be printed.
// pads[0]=zero_pad, pads[1]=space_pad, pads[2]=prefix_len
static int	ft_get_nbr_pads(char *s, int n, t_flags f, int *pads)
{
	int	len;

	len = ft_strlen(s);
	if (f.dot && f.precision == 0 && n == 0)
		len = 0;
	pads[2] = 0;
	if (n < 0 || f.plus || f.space)
		pads[2] = 1;
	pads[0] = f.precision - len;
	if (pads[0] < 0)
		pads[0] = 0;
	if (f.zero)
	{
		pads[0] = f.width - len - pads[2];
		if (pads[0] < 0)
			pads[0] = 0;
	}
	pads[1] = f.width - len - pads[0] - pads[2];
	if (pads[1] < 0)
		pads[1] = 0;
	return (len);
}

// Helper 2: Print the prefix (+, -, or space) and return its length.
static int	ft_print_prefix(int n, t_flags f)
{
	if (n < 0)
		return (write(1, "-", 1));
	if (f.plus)
		return (write(1, "+", 1));
	if (f.space)
		return (write(1, " ", 1));
	return (0);
}

/*
** ft_out_nbr handles all padding and prefix logic for d/i/u specifiers.
** 1. Resolve flags: '+' overrides ' ', while '-' or '.' override '0'.
** 2. ft_get_nbr_pads(s, n, f, pads):
**      Determines prefix length (for '-', '+', or ' ').
**      Calculates zero-padding based on precision or the '0' flag.
**      Calculates space-padding to meet the minimum width.
** 3. Prints in order: space-padding, prefix, zero-padding, number, and
**    finally trailing space-padding if left-aligned.
** Note: This function is long for Norminette. It could be split into
** helper functions to calculate padding or print prefixes if needed.
*/
int	ft_out_nbr(char *s, int n, t_flags f)
{
	int	count;
	int	len;
	int	pads[3];

	count = 0;
	if (f.plus)
		f.space = 0;
	if (f.minus || f.dot)
		f.zero = 0;
	len = ft_get_nbr_pads(s, n, f, pads);
	if (!f.minus)
		count += ft_print_padding(' ', pads[1]);
	count += ft_print_prefix(n, f);
	count += ft_print_padding('0', pads[0]);
	count += write(1, s, len);
	if (f.minus)
		count += ft_print_padding(' ', pads[1]);
	return (count);
}

int	ft_print_nbr(int n, t_flags flags)
{
	char	*s;
	int		count;
	long	nb;

	nb = n;
	if (nb < 0)
		nb = -nb;
	s = ft_utoa((unsigned int)nb);
	if (!s)
		return (0);
	count = ft_out_nbr(s, n, flags);
	free(s);
	return (count);
}

// For unsigned, '+' and ' ' flags are ignored.
int	ft_print_unsigned(unsigned int n, t_flags flags)
{
	char	*s;
	int		count;
	int		representative_n;

	flags.plus = 0;
	flags.space = 0;
	s = ft_utoa(n);
	if (!s)
		return (0);
	if (n == 0)
		representative_n = 0;
	else
		representative_n = 1;
	count = ft_out_nbr(s, representative_n, flags);
	free(s);
	return (count);
}
