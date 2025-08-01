/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:50:31 by anemet            #+#    #+#             */
/*   Updated: 2025/06/13 16:22:23 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

// return the address of last `c` in string `s`
// return (char *)(s + i) -- we need to cast return to (char *)
// because (s + i) is `const char *` and function return type is `char *`
// if `c` not in `s` then return pointer to '\0'
// otherwise return the `last` seen address of `c`
char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*last;

	i = 0;
	last = NULL;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			last = (char *)(s + i);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)(s + i));
	return (last);
}
