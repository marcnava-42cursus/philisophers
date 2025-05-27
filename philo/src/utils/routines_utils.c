/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:34:22 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/27 20:40:41 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_philo_action(t_philo *philo, char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->mtx_print);
	timestamp = get_time_ms() - philo->data->start_time;
	if (!is_dead(philo->data))
		printf("%ld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->data->mtx_print);
}

void	*one_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	print_philo_action(philo, FORK_MSG);
	msleep(philo->data->time_to_die);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	return (NULL);
}

int	is_dead(t_data *data)
{
	int	isdeath;

	pthread_mutex_lock(&data->mtx_death);
	isdeath = data->philo_death;
	pthread_mutex_unlock(&data->mtx_death);
	return (isdeath);
}

int	temporal_name(t_data *data)
{
	int	i;
	int	eaten;

	if (data->eat_limit == -1)
		return (0);
	i = 0;
	eaten = 1;
	pthread_mutex_lock(&data->mtx_eat);
	while (i < data->n_philos)
	{
		if (data->philos[i].eat_count < data->eat_limit)
		{
			eaten = 0;
			break ;
		}
		i++;
	}
	return (pthread_mutex_unlock(&data->mtx_eat), eaten);
}

int	check_philosopher(t_data *data)
{
	int		i;
	long	diff_eat_time;

	while (!is_dead(data) && !temporal_name(data))
	{
		i = 0;
		while (i < data->n_philos)
		{
			pthread_mutex_lock(&data->mtx_eat);
			diff_eat_time = get_time_ms() - data->philos[i].last_eat_time;
			pthread_mutex_unlock(&data->mtx_eat);
			if (diff_eat_time > data->time_to_die)
			{
				print_philo_action(&data->philos[i], DIED_MSG);
				pthread_mutex_lock(&data->mtx_death);
				data->philo_death = 1;
				pthread_mutex_unlock(&data->mtx_death);
				break ;
			}
			i++;
		}
		msleep(1);
	}
	return (0);
}
