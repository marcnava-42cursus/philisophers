/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:40:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/09 00:40:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"
#include <string.h>
#include <pthread.h>

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
int	parse_arguments(t_data *data, int argc, char **argv)
{
	if (!is_valid_number(argv[1]) || !is_valid_number(argv[2]) ||
		!is_valid_number(argv[3]) || !is_valid_number(argv[4]))
		return (1);
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
	{
		if (!is_valid_number(argv[5]))
			return (1);
		data->must_eat_count = ft_atoi(argv[5]);
	}
	if (data->nb_philos <= 0 || data->time_to_die <= 0 ||
		data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
		(argc == 6 && data->must_eat_count <= 0))
		return (1);
	return (0);
}

/* Initialize semaphores */
int	init_semaphores(t_data *data)
{
	/* Unlink semaphores first to ensure they are not already existing */
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_DEATH);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_ALL_ATE);

	/* Open semaphores */
	data->forks = sem_open(SEM_FORKS, O_CREAT, 0644, data->nb_philos);
	data->print_sem = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	data->death_sem = sem_open(SEM_DEATH, O_CREAT, 0644, 1);
	data->meal_sem = sem_open(SEM_MEAL, O_CREAT, 0644, 1);
	data->all_ate_sem = sem_open(SEM_ALL_ATE, O_CREAT, 0644, 0);

	if (data->forks == SEM_FAILED || data->print_sem == SEM_FAILED ||
		data->death_sem == SEM_FAILED || data->meal_sem == SEM_FAILED ||
		data->all_ate_sem == SEM_FAILED)
		return (1);
	return (0);
}

/* Initialize philosopher structures */
int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	data->pids = malloc(sizeof(pid_t) * data->nb_philos);
	if (!data->philos || !data->pids)
		return (1);
	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].eat_count = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].data = data;
		data->pids[i] = -1;
		i++;
	}
	return (0);
}

/* Initialize all data */
int	init_data(t_data *data, int argc, char **argv)
{
	memset(data, 0, sizeof(t_data));
	if (parse_arguments(data, argc, argv))
		return (1);
	data->someone_died = 0;
	if (init_semaphores(data))
	{
		cleanup(data);
		return (1);
	}
	if (init_philos(data))
	{
		cleanup(data);
		return (1);
	}
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

/* Print philosopher status with timestamp */
void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	sem_wait(philo->data->print_sem);
	timestamp = get_time_diff(philo->data->start_time);
	if (!check_death(philo->data) || strcmp(status, DIED) == 0)
		printf("%ld %d %s\n", timestamp, philo->id, status);
	if (strcmp(status, DIED) != 0)
		sem_post(philo->data->print_sem);
}

/* Check if simulation should end (someone died) */
bool	check_death(t_data *data)
{
	bool	result;

	sem_wait(data->death_sem);
	result = data->someone_died;
	sem_post(data->death_sem);
	return (result);
}

/* Monitor for philosopher death (runs in each philosopher process) */
void	*death_monitor(void *arg)
{
	t_philo	*philo;
	long	time_since_last_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->data->meal_sem);
		time_since_last_meal = get_time_diff(philo->last_meal_time);
		sem_post(philo->data->meal_sem);
		
		if (time_since_last_meal > philo->data->time_to_die)
		{
			print_status(philo, DIED);
			sem_wait(philo->data->death_sem);
			philo->data->someone_died = 1;
			sem_post(philo->data->death_sem);
			exit(1);
		}
		
		if (philo->data->must_eat_count != -1 && 
			philo->eat_count >= philo->data->must_eat_count)
		{
			sem_post(philo->data->all_ate_sem);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

/* Free all allocated memory and close/unlink semaphores */
void	cleanup(t_data *data)
{
	/* Close semaphores */
	if (data->forks != NULL)
		sem_close(data->forks);
	if (data->print_sem != NULL)
		sem_close(data->print_sem);
	if (data->death_sem != NULL)
		sem_close(data->death_sem);
	if (data->meal_sem != NULL)
		sem_close(data->meal_sem);
	if (data->all_ate_sem != NULL)
		sem_close(data->all_ate_sem);
	
	/* Unlink semaphores */
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_DEATH);
	sem_unlink(SEM_MEAL);
	sem_unlink(SEM_ALL_ATE);
	
	/* Free allocated memory */
	if (data->philos != NULL)
		free(data->philos);
	if (data->pids != NULL)
		free(data->pids);
}

/* Kill all child processes */
void	kill_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (data->pids[i] > 0)
			kill(data->pids[i], SIGKILL);
		i++;
	}
}

/* Start the simulation by creating processes for each philosopher */
int	start_simulation(t_data *data)
{
	int		i;
	pthread_t	death_thread;

	data->start_time = get_time();
	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		data->pids[i] = fork();
		
		if (data->pids[i] < 0)
		{
			kill_processes(data);
			return (1);
		}
		if (data->pids[i] == 0)
		{
			/* Child process - philosopher */
			if (pthread_create(&death_thread, NULL, death_monitor, &data->philos[i]))
				exit(1);
			pthread_detach(death_thread);
			philo_process(&data->philos[i]);
			exit(0);
		}
		usleep(100); /* Small delay to prevent race condition on fork creation */
		i++;
	}
	return (0);
}

/* Monitor philosophers for completion */
void	monitor_philos(t_data *data)
{
	int	status;
	int	completed;

	/* Wait for all philosophers to finish or one to die */
	if (data->must_eat_count != -1)
	{
		completed = 0;
		while (completed < data->nb_philos)
		{
			sem_wait(data->all_ate_sem);
			completed++;
		}
		/* All philosophers have eaten enough, kill all processes */
		kill_processes(data);
	}
	else
	{
		/* Wait for any philosopher to die */
		waitpid(-1, &status, 0);
		/* If one dies, kill all other processes */
		kill_processes(data);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s <np> <ttd> <tte> <tts> [tnte]\n", argv[0]);
		printf("np: Number of philosophers you are going to simulate\n");
		printf("ttd: Time to Die of each philosopher\n");
		printf("tte: Time each philosopher took to eat\n");
		printf("tts: Time each philosopher took to sleep\n");
		printf("tnte: [optional] Times each philosopher must to eat\n");
		return (1);
	}
	if (init_data(&data, argc, argv))
	{
		printf("Error: Invalid arguments or initialization failed\n");
		return (1);
	}
	if (start_simulation(&data))
	{
		printf("Error: Failed to create processes\n");
		cleanup(&data);
		return (1);
	}
	monitor_philos(&data);
	cleanup(&data);
	return (0);
}

