/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:30:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/09 00:30:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <semaphore.h>
# include <stdbool.h>
# include <limits.h>

# define FORK_TAKEN "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DIED "died"

# define SEM_FORKS "/sem_forks"
# define SEM_PRINT "/sem_print"
# define SEM_DEATH "/sem_death"
# define SEM_MEAL "/sem_meal"
# define SEM_ALL_ATE "/sem_all_ate"

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int		nb_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		must_eat_count;
	long	start_time;
	int		someone_died;
	sem_t	*forks;
	sem_t	*print_sem;
	sem_t	*death_sem;
	sem_t	*meal_sem;
	sem_t	*all_ate_sem;
	pid_t	*pids;
	t_philo	*philos;
}	t_data;

struct s_philo
{
	int		id;
	int		eat_count;
	long	last_meal_time;
	pid_t	pid;
	t_data	*data;
};

/* Main functions */
int		init_data(t_data *data, int argc, char **argv);
int		start_simulation(t_data *data);
void	monitor_philos(t_data *data);
void	cleanup(t_data *data);
void	kill_processes(t_data *data);

/* Philosopher routine functions */
void	philo_process(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

/* Utility functions */
int		ft_atoi(const char *str);
int		is_valid_number(const char *str);
int		parse_arguments(t_data *data, int argc, char **argv);
int		init_semaphores(t_data *data);
int		init_philos(t_data *data);
long	get_time(void);
long	get_time_diff(long start);
void	print_status(t_philo *philo, char *status);
void	msleep(long milliseconds);
bool	check_death(t_data *data);
void	*death_monitor(void *arg);
void	*eat_monitor(void *arg);

#endif

