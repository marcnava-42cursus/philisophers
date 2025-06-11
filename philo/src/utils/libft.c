/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:02:16 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/11 14:33:09 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Converts a string to an integer
 *
 * This function parses the input string `str` and converts it to an integer.
 * It skips any leading whitespace characters (spaces, tabs, etc.), handles
 * optional '+' or '-' signs, and processes numeric characters until a
 * non-numeric character is encountered. If the input string represents a number
 * that exceeds the range of an `int`, the behavior is undefined.
 *
 * @param str The input string to be converted to an integer.
 * 
 * @return The integer value represented by the string `str`. If the string does
 *         not contain a valid number, the return value is undefined.
 */
int	ft_atoi(const char *str)
{
	int		sign;
	long	result;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX || result * sign < INT_MIN)
			return (0);
		str++;
	}
	return ((int)(result * sign));
}
