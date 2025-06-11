/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:23:22 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/11 14:37:33 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	terminate_data(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->n_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->mtx_print);
	pthread_mutex_destroy(&data->mtx_death);
	pthread_mutex_destroy(&data->mtx_eat);
	if (data->philos)
		free(data->philos);
}
