/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:50:21 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/28 19:27:16 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc != 5 && argc != 6)
		ft_error(ERR_USAGE, argv);
	if (init_data(&data, argc, argv))
		ft_error(ERR_INIT, argv);
	if (simulate(&data))
		return (terminate_data(&data));
	check_philosopher(&data);
	i = 0;
	while (i < data.n_philos)
		pthread_join(data.philos[i++].thread, NULL);
	return (terminate_data(&data));
}
