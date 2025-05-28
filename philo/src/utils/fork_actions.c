/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_actions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:15:26 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/28 17:46:08 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_fork(t_philo *philo, int left_fork, int right_fork)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_philo_action(philo, FORK_MSG);
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_philo_action(philo, FORK_MSG);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_philo_action(philo, FORK_MSG);
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_philo_action(philo, FORK_MSG);
	}
}

void	drop_fork(t_philo *philo, int left_fork, int right_fork)
{
	pthread_mutex_unlock(&philo->data->forks[left_fork]);
	pthread_mutex_unlock(&philo->data->forks[right_fork]);
}
