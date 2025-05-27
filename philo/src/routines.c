/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:41:07 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/27 17:41:35 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_philo *philo)
{
	take_fork(philo, philo->left_fork, philo->right_fork);
	print_philo_action(philo, EAT_MSG);
	pthread_mutex_lock(&philo->data->mtx_eat);
	philo->last_eat_time = get_time_ms();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->mtx_eat);
	msleep(philo->data->time_to_eat);
	drop_fork(philo, philo->left_fork, philo->right_fork);
}

static void	philo_sleep(t_philo *philo)
{
	print_philo_action(philo, SLEEP_MSG);
	msleep(philo->data->time_to_sleep);
}

static void	philo_think(t_philo *philo)
{
	print_philo_action(philo, THINK_MSG);
	msleep(1);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->mtx_eat);
	philo->last_eat_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->mtx_eat);
	if (philo->data->n_philos == 1)
		return (one_philo_routine(philo));
	if (philo->id % 2 == 0)
		msleep(1);
	while (!is_dead(philo->data) && !temporal_name(philo->data))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
