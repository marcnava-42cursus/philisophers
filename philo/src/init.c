/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:18:07 by marcnava          #+#    #+#             */
/*   Updated: 2025/06/11 14:37:54 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_arguments(t_data *data, int argc, char **argv)
{
	if (!is_number(argv[1]) || !is_number(argv[2])
		|| !is_number(argv[3]) || !is_number(argv[4]))
		return (1);
	data->n_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->eat_limit = -1;
	if (argc == 6)
	{
		if (!is_number(argv[5]))
			return (1);
		data->eat_limit = ft_atoi(argv[5]);
	}
	if (data->n_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (argc == 6 && data->eat_limit <= 0))
		return (1);
	return (0);
}

static int	initialize_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->mtx_print, NULL))
		return (1);
	if (pthread_mutex_init(&data->mtx_death, NULL))
		return (1);
	if (pthread_mutex_init(&data->mtx_eat, NULL))
		return (1);
	return (0);
}

static int	generate_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(data->n_philos * sizeof(t_philo));
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->n_philos;
		data->philos[i].eat_count = 0;
		data->philos[i].last_eat_time = 0;
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	if (parse_arguments(data, argc, argv))
		return (1);
	data->philo_death = 0;
	if (initialize_mutexes(data))
	{
		printf("Error: Failed initializing mutexes");
		terminate_data(data);
		return (1);
	}
	if (generate_philosophers(data))
	{
		printf("Error: Failed initializing philosophers");
		terminate_data(data);
		return (1);
	}
	return (0);
}
