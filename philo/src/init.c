/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/09 00:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/neko.h"

/* Parse command line arguments */
int	parse_arguments(t_mochi *data, int argc, char **argv)
{
	if (!is_valid_number(argv[1]) || !is_valid_number(argv[2]) ||
		!is_valid_number(argv[3]) || !is_valid_number(argv[4]))
		return (1);
	data->num_nekos = ft_atoi(argv[1]);
	data->starve_time = ft_atoi(argv[2]);
	data->munching_time = ft_atoi(argv[3]);
	data->nap_time = ft_atoi(argv[4]);
	data->snack_quota = -1;
	if (argc == 6)
	{
		if (!is_valid_number(argv[5]))
			return (1);
		data->snack_quota = ft_atoi(argv[5]);
	}
	if (data->num_nekos <= 0 || data->starve_time <= 0 ||
		data->munching_time <= 0 || data->nap_time <= 0 ||
		(argc == 6 && data->snack_quota <= 0))
		return (1);
	return (0);
}

/* Initialize mutexes */
int	init_mutexes(t_mochi *data)
{
	int	i;

	data->chopsticks = malloc(sizeof(pthread_mutex_t) * data->num_nekos);
	if (!data->chopsticks)
		return (1);
	i = 0;
	while (i < data->num_nekos)
	{
		if (pthread_mutex_init(&data->chopsticks[i], NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->meow_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->faint_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->munch_mutex, NULL))
		return (1);
	return (0);
}

/* Initialize nekosopher structures */
int	spawn_nekos(t_mochi *data)
{
	int	i;

	data->nekos = malloc(sizeof(t_neko) * data->num_nekos);
	if (!data->nekos)
		return (1);
	i = 0;
	while (i < data->num_nekos)
	{
		data->nekos[i].id = i + 1;
		data->nekos[i].left_chopstick = i;
		data->nekos[i].right_chopstick = (i + 1) % data->num_nekos;
		data->nekos[i].num_noms = 0;
		data->nekos[i].last_nom_time = 0;
		data->nekos[i].data = data;
		i++;
	}
	return (0);
}

/* Initialize all data */
int	init_mochi(t_mochi *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_mochi));
	if (parse_arguments(data, argc, argv))
		return (1);
	data->neko_fainted = 0;
	if (init_mutexes(data))
	{
		tidy_up(data);
		return (1);
	}
	if (spawn_nekos(data))
	{
		tidy_up(data);
		return (1);
	}
	return (0);
}

/* Free all allocated memory and destroy mutexes */
void	tidy_up(t_mochi *data)
{
	int	i;

	if (data->chopsticks)
	{
		i = 0;
		while (i < data->num_nekos)
		{
			pthread_mutex_destroy(&data->chopsticks[i]);
			i++;
		}
		free(data->chopsticks);
	}
	pthread_mutex_destroy(&data->meow_mutex);
	pthread_mutex_destroy(&data->faint_mutex);
	pthread_mutex_destroy(&data->munch_mutex);
	if (data->nekos)
		free(data->nekos);
}

