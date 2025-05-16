/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 01:25:00 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/09 00:42:00 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEKO_H
# define NEKO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <string.h>
# include <limits.h>

/* Status messages */
# define GRABBED_CHOPSTICK "has taken a fork"
# define MUNCHING "is eating"
# define NAPPING "is sleeping"
# define PONDERING "is thinking"
# define FAINTED "died"

typedef struct s_neko t_neko;

typedef struct s_mochi
{
	int				num_nekos;
	int				starve_time;
	int				munching_time;
	int				nap_time;
	int				snack_quota;
	long			start_time;
	int				neko_fainted;
	pthread_mutex_t	*chopsticks;
	pthread_mutex_t	meow_mutex;
	pthread_mutex_t	faint_mutex;
	pthread_mutex_t	munch_mutex;
	t_neko			*nekos;
}	t_mochi;

struct s_neko
{
	int			id;
	int			left_chopstick;
	int			right_chopstick;
	int			num_noms;
	long		last_nom_time;
	pthread_t	thread;
	t_mochi		*data;
};

/* Main functions */
int		init_mochi(t_mochi *data, int argc, char **argv);
int		start_simulation(t_mochi *data);
int		watch_nekos(t_mochi *data);
void	tidy_up(t_mochi *data);

/* Philosopher routine functions */
void	*neko_life(void *arg);
void	neko_munch(t_neko *neko);
void	neko_nap(t_neko *neko);
void	neko_ponder(t_neko *neko);

/* Utility functions */
int		ft_atoi(const char *str);
int		is_valid_number(const char *str);
int		parse_arguments(t_mochi *data, int argc, char **argv);
int		init_mutexes(t_mochi *data);
int		spawn_nekos(t_mochi *data);
long	get_time(void);
long	get_time_diff(long start);
void	print_status(t_neko *neko, char *status);
void	msleep(long milliseconds);
bool	is_neko_fainted(t_mochi *data);
bool	all_nekos_full(t_mochi *data);

#endif
