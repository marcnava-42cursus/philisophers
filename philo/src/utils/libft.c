/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:02:16 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/23 13:44:08 by marcnava         ###   ########.fr       */
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
	int		i;
	int		sign;
	long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return ((int)(res * sign));
}

/**
 * @brief Allocates memory for an array of `count` elements of `size` bytes each
 *        and initializes all bytes in the allocated memory to zero.
 *
 * This function is a custom implementation of the standard `calloc` function.
 * It allocates memory using `malloc` and then sets all allocated memory to zero
 * using `memset`.
 *
 * @param count The number of elements to allocate.
 * @param size The size of each element in bytes.
 * @return A pointer to the allocated memory, or NULL if the allocation fails.
 */
void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = (void *)malloc(count * size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, count * size);
	return (ptr);
}

/**
 * @brief Safely frees a dynamically allocated memory block and sets the pointer
 *        to NULL.
 *
 * This function checks if the given pointer to a pointer is not NULL and if the
 * memory it points to is also not NULL. If both conditions are met,
 * it frees the allocated memory and sets the pointer to NULL
 * to prevent dangling pointers.
 *
 * @param ptr A double pointer to the memory block to be freed.
 *            The pointer itself will be set to NULL after the memory is freed.
 */
void	ft_free(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

/**
 * @brief Copies memory from a source location to a destination location.
 * 
 * This function copies `n` bytes from the memory area pointed to by `src`
 * to the memory area pointed to by `dest`. The memory areas must not overlap.
 * If either `dest` or `src` is NULL, the function returns NULL.
 * 
 * @param dest Pointer to the destination memory area where the content is
 *             to be copied.
 * @param src Pointer to the source memory area from which the content is
 *            to be copied.
 * @param n Number of bytes to copy from `src` to `dest`.
 * 
 * @return A pointer to the destination memory area `dest`,
 *         or NULL if both `dest` and `src` are NULL.
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

/**
 * @brief Duplicates a string by allocating memory and copying the content.
 * 
 * This function creates a new string that is a duplicate of the input
 * string `s`.
 * It allocates memory for the new string, copies the content of `s` into it,
 * and returns a pointer to the newly allocated string.
 * 
 * @param s The input string to duplicate. If `s` is NULL,
 *          the function returns NULL.
 * 
 * @return A pointer to the newly allocated string containing the duplicate
 *         of `s`. If memory allocation fails or if `s` is NULL,
 *         the function returns NULL.
 * 
 * @note The returned string must be freed by the caller to avoid memory leaks.
 */
char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	len;

	if (!s)
		return (NULL);
	len = 0;
	while (s[len])
		len++;
	len++;
	if (!len)
		return (NULL);
	str = (char *)ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, s, len);
	return (str);
}
