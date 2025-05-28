/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:38:01 by marcnava          #+#    #+#             */
/*   Updated: 2025/05/28 17:43:50 by marcnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Retrieves the current time in milliseconds.
 *
 * This function uses the `gettimeofday` system call to obtain the current time
 * and converts it to milliseconds. The time is calculated as the number of 
 * seconds since the Epoch (1970-01-01 00:00:00 UTC) multiplied by 1000, plus 
 * the number of microseconds divided by 1000.
 *
 * @return The current time in milliseconds as a long integer.
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Pauses the execution of the program for a specified number of
 *        milliseconds.
 *
 * This function uses a combination of `get_time_ms()` and `usleep()` to achieve
 * a delay close to the specified duration in milliseconds.
 * It continuously checks the elapsed time and ensures that the program sleeps
 * for the desired duration.
 *
 * @param milliseconds The number of milliseconds to sleep.
 *
 * @note The function uses `usleep(500)` to avoid busy-waiting,
 *       which helps reduce CPU usage during the sleep period.
 *       The precision of the sleep duration depends on the accuracy
 *       of `get_time_ms()` and the system's scheduling behavior.
 */
void	msleep(long milliseconds)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < milliseconds)
		usleep(200);
}
