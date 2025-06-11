/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:50:21 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/11 14:37:26 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s <np> <ttd> <tte> <tts> [tnte]\n", argv[0]);
		printf("\tnp: Number of philosopher you are going to simulate\n");
		printf("\tttd: Time to Die of each philosopher\n");
		printf("\ttte: Time each philosopher took to eat\n");
		printf("\ttts: Time each philosopher took to sleep\n");
		printf("\ttnte [optional]: Times each philosopher must to eat\n");
		return (1);
	}
	if (init_data(&data, argc, argv))
	{
		printf("Error: setting up the simulation");
		return (1);
	}
	if (simulate(&data))
	{
		printf("Error: Failed to create threads");
		terminate_data(&data);
		return (1);
	}
	check_philosopher(&data);
	i = 0;
	while (i < data.n_philos)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	terminate_data(&data);
	return (1);
}
