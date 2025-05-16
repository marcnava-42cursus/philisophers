/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:20:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/09 01:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/neko.h"

/* Take chopsticks according to nekosopher ID to prevent deadlock */
static void	grab_chopsticks(t_neko *neko, int left, int right)
{
	if (neko->id % 2 == 0)
	{
		pthread_mutex_lock(&neko->data->chopsticks[right]);
		print_status(neko, GRABBED_CHOPSTICK);
		pthread_mutex_lock(&neko->data->chopsticks[left]);
		print_status(neko, GRABBED_CHOPSTICK);
	}
	else
	{
		pthread_mutex_lock(&neko->data->chopsticks[left]);
		print_status(neko, GRABBED_CHOPSTICK);
		pthread_mutex_lock(&neko->data->chopsticks[right]);
		print_status(neko, GRABBED_CHOPSTICK);
	}
}

/* Release chopsticks after eating */
static void	drop_chopsticks(t_neko *neko, int left, int right)
{
	pthread_mutex_unlock(&neko->data->chopsticks[left]);
	pthread_mutex_unlock(&neko->data->chopsticks[right]);
}

/* Eating routine for a nekosopher */
void	neko_munch(t_neko *neko)
{
	grab_chopsticks(neko, neko->left_chopstick, neko->right_chopstick);
	print_status(neko, MUNCHING);
	
	pthread_mutex_lock(&neko->data->munch_mutex);
	neko->last_nom_time = get_time();
	neko->num_noms++;
	pthread_mutex_unlock(&neko->data->munch_mutex);
	
	msleep(neko->data->munching_time);
	drop_chopsticks(neko, neko->left_chopstick, neko->right_chopstick);
}

/* Sleeping routine for a nekosopher */
void	neko_nap(t_neko *neko)
{
	print_status(neko, NAPPING);
	msleep(neko->data->nap_time);
}

/* Thinking routine for a nekosopher */
void	neko_ponder(t_neko *neko)
{
	print_status(neko, PONDERING);
	/* Small delay to prevent CPU hogging in case of very small time values */
	usleep(100);
}

/* Special case for single nekosopher that can't eat */
static void	lonely_neko_life(t_neko *neko)
{
	pthread_mutex_lock(&neko->data->chopsticks[neko->left_chopstick]);
	print_status(neko, GRABBED_CHOPSTICK);
	msleep(neko->data->starve_time);
	pthread_mutex_unlock(&neko->data->chopsticks[neko->left_chopstick]);
}

/* Main routine for each nekosopher thread */
void	*neko_life(void *arg)
{
	t_neko	*neko;

	neko = (t_neko *)arg;
	/* Set initial meal time to start time */
	pthread_mutex_lock(&neko->data->munch_mutex);
	neko->last_nom_time = neko->data->start_time;
	pthread_mutex_unlock(&neko->data->munch_mutex);
	
	/* Special case for single nekosopher */
	if (neko->data->num_nekos == 1)
	{
		lonely_neko_life(neko);
		return (NULL);
	}
	
	/* Even nekosophers start with small delay to avoid deadlocks */
	if (neko->id % 2 == 0)
		usleep(1000);

	/* Main nekosopher life cycle */
	while (!is_neko_fainted(neko->data) && !all_nekos_full(neko->data))
	{
		neko_munch(neko);
		neko_nap(neko);
		neko_ponder(neko);
	}

	return (NULL);
}
