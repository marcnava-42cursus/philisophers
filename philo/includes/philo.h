/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 19:57:22 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/28 16:47:58 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// Headers
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

// Messages
# define FORK_MSG	"has taken a fork"
# define EAT_MSG	"is eating"
# define SLEEP_MSG	"is sleeping"
# define THINK_MSG	"is thinking"
# define DIED_MSG	"died"

# define EXIT_OK 0
# define EXIT_KO 1

// Type definitions
typedef struct s_philo	t_philo;
typedef struct s_data	t_data;
typedef enum e_errors	t_errors;

// Enums
enum e_error
{
	ERR_USAGE = 1,
	ERR_INIT = 2
};

// Structs
struct s_philo
{
	int			id;
	int			left_fork;
	int			right_fork;
	int			eat_count;
	int			last_eat_time;
	pthread_t	thread;
	t_data		*data;
};

struct s_data
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_limit;
	long			start_time;
	int				philo_death;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mtx_print;
	pthread_mutex_t	mtx_death;
	pthread_mutex_t	mtx_eat;
	t_philo			*philos;
};

// Libft
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
void	ft_free(void **ptr);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strdup(const char *s);

// Errors
void	ft_error(int error_code, char **argv);

// Utils
void	print_debug(char *message, int status, int show);
void	print_philo_action(t_philo *philo, char *action);
void	*one_philo_routine(t_philo *philo);
long	get_time_ms(void);
void	msleep(long milliseconds);
int		is_dead(t_data *data);
int		temporal_name(t_data *data);
void	take_fork(t_philo *philo, int left_fork, int right_fork);
void	drop_fork(t_philo *philo, int left_fork, int right_fork);
int		check_philosopher(t_data *data);

// Philosophers
int		simulate(t_data *data);
int		init_data(t_data *data, int argc, char **argv);
int		terminate_data(t_data *data);

#endif