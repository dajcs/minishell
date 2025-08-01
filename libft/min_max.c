/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_max.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:58:49 by anemet            #+#    #+#             */
/*   Updated: 2025/07/17 13:02:55 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// return the minimum of two integers
int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

// return the maximum of two integers
int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

// return the minimum of two floats
float	minf(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

// return the maximum of two floats
float	maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}
