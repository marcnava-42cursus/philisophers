/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:50:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/09 00:50:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

/* Philosopher takes forks and eats */
void	philo_eat(t_philo *philo)
{
	/* Take two forks using semaphore */
	sem_wait(philo->data->forks);
	print_status(philo, FORK_TAKEN);
	sem_wait(philo->data->forks);
	print_status(philo, FORK_TAKEN);
	
	/* Update eating status and timestamp */
	sem_wait(philo->data->meal_sem);
	philo->last_meal_time = get_time();
	philo->eat_count++;
	sem_post(philo->data->meal_sem);
	
	print_status(philo, EATING);
	msleep(philo->data->time_to_eat);
	
	/* Release forks */
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

/* Philosopher sleeps */
void	philo_sleep(t_philo *philo)
{
	print_status(philo, SLEEPING);
	msleep(philo->data->time_to_sleep);
}

/* Philosopher thinks */
void	philo_think(t_philo *philo)
{
	print_status(philo, THINKING);
	/* Add a small delay to prevent CPU-intensive spinning */
	usleep(100);
}

/* Handle case with only one philosopher */
static void	solo_philosopher(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_status(philo, FORK_TAKEN);
	msleep(philo->data->time_to_die);
	sem_post(philo->data->forks);
}

/* Main routine for each philosopher process */
void	philo_process(t_philo *philo)
{
	/* Handle special case with only one philosopher */
	if (philo->data->nb_philos == 1)
	{
		solo_philosopher(philo);
		exit(0);
	}
	
	/* Stagger philosopher start times to prevent deadlock */
	if (philo->id % 2 == 0)
		usleep(1000);
	
	/* Main philosopher life cycle */
	while (!check_death(philo->data))
	{
		/* Check if philosopher has eaten enough */
		if (philo->data->must_eat_count != -1 && 
			philo->eat_count >= philo->data->must_eat_count)
			break;
		
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	
	/* If all meals are eaten, exit normally */
	if (philo->data->must_eat_count != -1 && 
		philo->eat_count >= philo->data->must_eat_count)
		exit(0);
	
	/* If simulation ended due to death, exit with error code */
	exit(1);
}

