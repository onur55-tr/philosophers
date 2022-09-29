/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odursun <odursun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:43:14 by odursun           #+#    #+#             */
/*   Updated: 2022/09/19 16:43:15 by odursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Created by Onur Dursun on 9/12/22.

#include "philosophers.h"

/**
 * It checks if the amount of philosophers is 1, and if it is,
 * it prints that the philosopher has taken a fork,
 * sleep for the time to die, and then prints that the philosopher has died
 *
 * @param ins a pointer to the ins structure
 *
 * @return 1 if the amount of philosophers is 1.
 */
int	control_philo(t_ins *ins)
{
	ins->philo->s_time = get_time();
	if (ins->amount == 1)
	{
		printf("%zu\t%zu has taken a fork\n", get_time() - ins->philo->s_time, \
			ins->philo->who);
		ft_usleep(ins->tt_die);
		printf("%zu\t%zu died", get_time() - ins->philo->s_time, \
			ins->philo->who);
		return (1);
	}
	return (0);
}

/*
 * Getting the current time in milliseconds.
 *
 * The number of seconds since the
 * Epoch, 1970-01-01 00:00:00 +0000 (UTC).
*/
size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * It waits for a given amount of time
 *
 * @param ms The amount of time to sleep in milliseconds.
 */
void	ft_usleep(size_t ms)
{
	size_t	end;

	end = get_time() + ms;
	while (get_time() < end)
		usleep(100);
}

int	ft_atoi(const char *str)
{
	int	res;

	res = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+')
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			res = res * 10 + *str++ - '0';
		else
			return (0);
	}
	if (res >= __INT_MAX__)
		return (0);
	return (res);
}

void	ft_clear(t_ins *ins)
{
	if (ins->philo)
		free(ins->philo);
	pthread_mutex_destroy(&ins->write_mutex);
	pthread_mutex_destroy(ins->frk_mutex);
	pthread_mutex_destroy(&ins->die_mutex);
}
