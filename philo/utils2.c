/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 23:52:33 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex))
		return (1);
	return (0);
}

int	unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex))
		return (1);
	return (0);
}

int	printa(char *str, t_philo *philo)
{
	if (lock(&philo->info->print))
		return (2);
	printf("%d %d %s", get_time(philo->info->start_time), philo->id, str);
	if (*str == 'd')
		return (2);
	if (unlock(&philo->info->print))
		return (2);
	return (1);
}

int	update_meals(t_philo *philo_tmp)
{
	if (lock(&philo_tmp->info->check))
		return (1);
	if (philo_tmp->nb_meals != -11 && --philo_tmp->nb_meals <= 0)
	{
		if (unlock(&philo_tmp->fork))
			return (1);
		if (philo_tmp->id == philo_tmp->info->nb_philo)
		{
			if (unlock(&(philo_tmp - philo_tmp->info->nb_philo + 1)->fork))
				return (1);
		}
		else if (unlock(&(philo_tmp + 1)->fork))
			return (1);
		if (unlock(&philo_tmp->info->check))
			return (1);
		return (1);
	}
	if (unlock(&philo_tmp->info->check))
		return (1);
	return (0);
}

int	complete_routine(t_philo *philo_tmp)
{
	if (printa("phil is eating\n", philo_tmp) == 2)
		return (1);
	ssleep(philo_tmp->info->time_to_eat);
	if (update_meals(philo_tmp))
		return (1);
	if (printa("phil is sleeping\n", philo_tmp) == 2)
		return (1);
	if (unlock(&philo_tmp->fork))
		return (1);
	if (philo_tmp->id == philo_tmp->info->nb_philo)
	{
		if (unlock(&(philo_tmp - philo_tmp->info->nb_philo + 1)->fork))
			return (1);
	}
	else if (unlock(&(philo_tmp + 1)->fork))
		return (1);
	ssleep(philo_tmp->info->time_to_sleep);
	if (printa("phil is thinking\n", philo_tmp) == 2)
		return (1);
	return (0);
}
