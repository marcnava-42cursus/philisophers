/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:37:40 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/20 16:38:42 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#define ERROR 1
#define WARNING 2
#define INFO 3

void	print_debug(char *message, int status, int show)
{
	char	*color;

	if (show == 1)
	{
		if (status == ERROR)
			color = ft_strdup("\e[0;31m· 🗙  · ");
		else if (status == WARNING)
			color = ft_strdup("\e[0;33m· ⚠  · ");
		else if (status == INFO)
			color = ft_strdup("\e[0;36m· 🛈  · ");
		else
			color = ft_strdup("\e[0m");
		printf("%s%s\e[0m\n", color, message);
		ft_free((void **)&color);
	}
	return ;
}
