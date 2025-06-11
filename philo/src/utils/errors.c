/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:55:14 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/11 14:33:04 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_usage(char *name)
{
	printf("Usage: %s <np> <ttd> <tte> <tts> [tnte]\n", name);
	printf("\tnp: Number of philosopher you are going to simulate\n");
	printf("\tttd: Time to Die of each philosopher\n");
	printf("\ttte: Time each philosopher took to eat\n");
	printf("\ttts: Time each philosopher took to sleep\n");
	printf("\ttnte [optional]: Times each philosopher must to eat\n");
	exit(EXIT_KO);
}

void	ft_error(int error_code, char **argv)
{
	if (error_code == ERR_USAGE)
		print_usage(argv[0]);
	if (error_code == ERR_INIT)
		printf("Error setting up the simulation");
	exit(EXIT_KO);
}
