/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:40:41 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/03 01:33:58 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/neko.h"

/* Check if a string is a valid positive number */
int	is_valid_number(const char *str)
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

/* Convert string to integer with validation */
int	ft_atoi(const char *str)
{
	int		sign;
	long	result;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX || result * sign < INT_MIN)
			return (0);
		str++;
	}
	return ((int)(result * sign));
}

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
	printf("DEBUG: Starting parse_arguments\n");
	if (parse_arguments(data, argc, argv))
		return (1);
	printf("DEBUG: parse_arguments successful, num_nekos=%d, ttd=%d, tte=%d, tts=%d\n", 
		data->num_nekos, data->starve_time, data->munching_time, data->nap_time);
	
	data->neko_fainted = 0;
	printf("DEBUG: Starting init_mutexes\n");
	if (init_mutexes(data))
	{
		printf("DEBUG: init_mutexes failed\n");
		tidy_up(data);
		return (1);
	}
	printf("DEBUG: init_mutexes successful\n");
	
	printf("DEBUG: Starting spawn_nekos\n");
	if (spawn_nekos(data))
	{
		printf("DEBUG: spawn_nekos failed\n");
		tidy_up(data);
		return (1);
	}
	printf("DEBUG: spawn_nekos successful\n");
	
	return (0);
}

/* Get current time in milliseconds */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* Calculate time difference in milliseconds */
long	get_time_diff(long start)
{
	return (get_time() - start);
}

/* Sleep for the specified number of milliseconds */
void	msleep(long milliseconds)
{
	long	start;

	start = get_time();
	while (get_time_diff(start) < milliseconds)
		usleep(500);
}

/* Print nekosopher status with timestamp */
void	print_status(t_neko *neko, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&neko->data->meow_mutex);
	timestamp = get_time_diff(neko->data->start_time);
	if (!is_neko_fainted(neko->data))
		printf("%ld %d %s\n", timestamp, neko->id, status);
	pthread_mutex_unlock(&neko->data->meow_mutex);
}

/* Check if simulation should end (someone died) */
bool	is_neko_fainted(t_mochi *data)
{
	bool	result;

	pthread_mutex_lock(&data->faint_mutex);
	result = data->neko_fainted;
	pthread_mutex_unlock(&data->faint_mutex);
	return (result);
}

/* Check if all nekosophers have eaten enough */
bool	all_nekos_full(t_mochi *data)
{
	int		i;
	bool	all_ate;

	if (data->snack_quota == -1)
		return (false);
	i = 0;
	all_ate = true;
	pthread_mutex_lock(&data->munch_mutex);
	while (i < data->num_nekos)
	{
		if (data->nekos[i].num_noms < data->snack_quota)
		{
			all_ate = false;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&data->munch_mutex);
	return (all_ate);
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

/* Start the simulation by creating threads for each nekosopher */
int	start_simulation(t_mochi *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_nekos)
	{
		data->nekos[i].last_nom_time = data->start_time;
		if (pthread_create(&data->nekos[i].thread, NULL,
				neko_life, &data->nekos[i]))
			return (1);
		i++;
	}
	return (0);
}

/* Monitor nekosophers for death or completion */
int	watch_nekos(t_mochi *data)
{
	int		i;
	long	time_since_last_meal;

	while (!is_neko_fainted(data) && !all_nekos_full(data))
	{
		i = 0;
		while (i < data->num_nekos)
		{
			pthread_mutex_lock(&data->munch_mutex);
			time_since_last_meal = get_time_diff(data->nekos[i].last_nom_time);
			pthread_mutex_unlock(&data->munch_mutex);
			if (time_since_last_meal > data->starve_time)
			{
				print_status(&data->nekos[i], FAINTED);
				pthread_mutex_lock(&data->faint_mutex);
				data->neko_fainted = 1;
				pthread_mutex_unlock(&data->faint_mutex);
				break ;
			}
			i++;
		}
		usleep(1000);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_mochi	data;
	int		i;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s <np> <ttd> <tte> <tts> [tnte]\n", argv[0]);
		printf("np: Number of nekosophers you are going to simulate\n");
		printf("ttd: Time to Die of each nekosopher\n");
		printf("tte: Time each nekosopher took to eat\n");
		printf("tts: Time each nekosopher took to sleep\n");
		printf("tnte: [optional] Times each nekosopher must to eat\n");
		return (1);
	}
	if (init_mochi(&data, argc, argv))
	{
		printf("Error: Invalid arguments or initialization failed\n");
		return (1);
	}
	if (start_simulation(&data))
	{
		printf("Error: Failed to create threads\n");
		tidy_up(&data);
		return (1);
	}
	watch_nekos(&data);
	i = 0;
	while (i < data.num_nekos)
	{
		pthread_join(data.nekos[i].thread, NULL);
		i++;
	}
	tidy_up(&data);
	return (0);
}
