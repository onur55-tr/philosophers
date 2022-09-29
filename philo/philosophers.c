/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odursun <odursun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:42:56 by odursun           #+#    #+#             */
/*   Updated: 2022/09/19 16:42:57 by odursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Created by Onur Dursun on 9/12/22.

#include "philosophers.h"

static void	ft_message(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->ins->write_mutex);
	pthread_mutex_lock(&philo->ins->die_mutex);
	if (!philo->ins->death)
		printf("%zu\t%zu %s\n", get_time() - philo->s_time, \
		philo->who, message);
	pthread_mutex_unlock(&philo->ins->die_mutex);
	pthread_mutex_unlock(&philo->ins->write_mutex);
}

int	ft_sleeping(t_philo *philo)
{
	ft_message(philo, "is sleeping");
	ft_usleep(philo->ins->tt_sleep);
	ft_message(philo, "is thinking");
	return (0);
}

int	ft_eating(t_philo *philo)
{
	ft_message(philo, "is eating");
	if (philo->ins->meals != -1)
		philo->meals += 1;
	pthread_mutex_lock(&philo->ins->meals_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->ins->meals_mutex);
	ft_usleep(philo->ins->tt_eat);
	pthread_mutex_unlock(&philo->ins->frk_mutex[philo->r_fork]);
	pthread_mutex_unlock(&philo->ins->frk_mutex[philo->l_fork]);
	return (0);
}

int	ft_take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->ins->frk_mutex[philo->r_fork]);
	ft_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->ins->frk_mutex[philo->l_fork]);
	ft_message(philo, "has taken a fork");
	return (0);
}

int	main(int ac, char **av)
{
	t_ins	ins;

	if (init_arg(&ins, ac, av) || control_philo(&ins) || start_threads(&ins))
		return (1);
	ft_clear(&ins);
	return (0);
}
