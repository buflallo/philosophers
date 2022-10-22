/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 23:52:19 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time(int star_time)
{
	struct timeval	tv;
	int				time;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (time / 1000 - star_time);
}

void	ssleep(int time)
{
	int	start;

	start = get_time(0);
	while (get_time(start) < time)
		usleep(100);
}

void	destroy_mutexes(t_philo *philo)
{
	int	i;

	i = 0;
	if (pthread_mutex_destroy(&philo->info->print))
		return ;
	if (pthread_mutex_destroy(&philo->info->check))
		return ;
	while (i < philo->info->nb_philo)
	{
		if (pthread_mutex_destroy(&philo[i].fork))
			return ;
		i++;
	}
}

int	check_death(int l_eat, int t_die, t_philo *pilo)
{
	if (get_time(0) - l_eat >= t_die)
	{
		printa("died\n", pilo);
		destroy_mutexes(pilo);
		return (1);
	}
	return (0);
}

int	check_meals(t_philo *philo, t_info *info)
{
	int	i;
	int	b;

	i = 0;
	b = 0;
	if (info->nb_philo < 1)
		return (1);
	lock(&info->check);
	while (i < info->nb_philo)
	{
		if (philo[i].nb_meals == 0)
		{
			b++;
		}
		i++;
	}
	if (b == info->nb_philo)
	{
		destroy_mutexes(philo);
		return (1);
	}
	unlock(&info->check);
	return (0);
}
