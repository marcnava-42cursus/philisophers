/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:23:22 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/23 14:13:39 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	terminate_data(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->n_philos)
			pthread_mutex_destroy(&data->forks[i++]);
		ft_free((void **)&(data->forks));
	}
	pthread_mutex_destroy(&data->mtx_death);
	pthread_mutex_destroy(&data->mtx_eat);
	pthread_mutex_destroy(&data->mtx_print);
	if (data->philos)
		ft_free((void **)&(data->philos));
	return (EXIT_KO);
}
