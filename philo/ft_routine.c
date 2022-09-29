/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odursun <odursun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:42:36 by odursun           #+#    #+#             */
/*   Updated: 2022/09/19 16:42:37 by odursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Created by Onur Dursun on 9/12/22.

#include "philosophers.h"

/**
 * It takes the forks, eats, sleeps, and then repeats
 *
 * @param v_philo a void pointer to the t_philo structure
 *
 * @return Returns zero whether routine() succeeds or not.
 */
void	*routine(void *v_philo)
{
	t_philo	*philo;

	philo = (t_philo *)v_philo;
	philo->s_time = get_time();
	pthread_mutex_lock(&philo->ins->meals_mutex);
	philo->last_meal = philo->s_time;
	pthread_mutex_unlock(&philo->ins->meals_mutex);
	while (1)
	{
		if (philo->ins->meals != -1 && philo->meals == philo->ins->meals)
			break ;
		ft_take_forks(philo);
		ft_eating(philo);
		ft_sleeping(philo);
		pthread_mutex_lock(&philo->ins->die_mutex);
		if (philo->ins->death)
		{
			pthread_mutex_unlock(&philo->ins->die_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->ins->die_mutex);
	}
	return (NULL);
}

static int	ft_watch_utils(t_ins *ins, int i)
{
	pthread_mutex_lock(&ins->meals_mutex);
	if (get_time() - ins->philo[i].last_meal >= ins->tt_die)
	{
		pthread_mutex_unlock(&ins->meals_mutex);
		pthread_mutex_lock(&ins->die_mutex);
		ins->death = 1;
		pthread_mutex_unlock(&ins->die_mutex);
		pthread_mutex_lock(&ins->write_mutex);
		if (ins->meals != ins->philo[i].meals)
			printf("%zu\t%zu died\n", get_time() - ins->philo[i].s_time, \
			ins->philo[i].who);
		pthread_mutex_unlock(&ins->write_mutex);
		return (1);
	}
	else
		return (0);
}

static void	ft_watch(t_ins *ins)
{
	size_t	i;

	i = 0;
	while (i < ins->amount)
	{
		if (ft_watch_utils(ins, i))
			break ;
		else
			pthread_mutex_unlock(&ins->meals_mutex);
		i++;
		if (i == ins->amount)
			i = 0;
		usleep(50);
	}
}

/**
 * It creates threads for each philosopher, and then waits for them to finish
 *
 * @param ins structure
 *
 * @return  If successful, the start_threads() function
 * will return zero.  Otherwise an error number will be
 * returned to indicate the error.
*/
int	start_threads(t_ins *ins)
{
	size_t	i;

	i = 0;
	while (i < ins->amount)
	{
		if (pthread_create(&ins->philo[i].th, NULL, &routine, &ins->philo[i]))
			return (1);
		i += 2;
		usleep(50);
	}
	i = 1;
	while (i < ins->amount)
	{
		if (pthread_create(&ins->philo[i].th, NULL, &routine, &ins->philo[i]))
			return (1);
		i += 2;
		usleep(50);
	}
	ft_watch(ins);
	i = -1;
	while (++i < ins->amount)
		if (pthread_join(ins->philo[i].th, NULL))
			return (1);
	return (0);
}
