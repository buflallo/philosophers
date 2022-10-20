/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/18 22:39:56 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex))
		return(printf("mutex lock failed\n"));
	return (0);
}

int	unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex))
		return(printf("mutex unlock failed"));
	return (0);
}

static int printa(char *str, t_philo *philo)
{
	if (lock(&philo->info->print) == 2)
		return(2);
	printf("%d %d %s", get_time(philo->info->start_time), philo->id, str);
	if (*str == 'd')
		return (2);
	if(unlock(&philo->info->print) == 2) 
		return (2);
	return (1);
}

void	*routine(void *philo)
{
	t_philo *philo_tmp;

	philo_tmp = (t_philo *)philo;
	while (1)
	{
		if (lock(&philo_tmp->left_fork))
			return (NULL);
		
		if (printa("phil Take left\n", philo_tmp) == 2)
			return (NULL);

		if (lock(philo_tmp->right_fork))
			return (NULL);
		if (printa("phil Take Right\n", philo_tmp) == 2)
			return (NULL);

		if (lock(&philo_tmp->info->check))
			return (NULL);
		philo_tmp->last_eat = get_time(0);
		if (unlock(&philo_tmp->info->check))
			return (NULL);

		printa("phil is eating\n", philo_tmp);
		if (lock(&philo_tmp->info->check))
			return (NULL);
		philo_tmp->nb_meals--;
		if (unlock(&philo_tmp->info->check))
			return (NULL);
		
		ssleep(philo_tmp->info->time_to_eat);

		
		if (unlock(philo_tmp->right_fork))
			return (NULL);
		if (unlock(&philo_tmp->left_fork))
			return (NULL);

		printa("phil is sleeping\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_sleep);

		printa("phil is thinking\n", philo_tmp);
	}
	return (NULL);
}

void	destroy_mutexes(t_philo *philo)
{
	int	i = 0;

	if (pthread_mutex_destroy(&philo->info->print))
		return ;
	if (pthread_mutex_destroy(&philo->info->check))
		return ;
	while (i < philo->info->nb_philo)
	{
		if (pthread_mutex_destroy(&philo[i].left_fork))
			return ;
		i++;
	}
}

static int	check_death(int l_eat, int t_die, t_philo *pilo)
{
	if (get_time(0) - l_eat >= t_die)
	{
		printa("died\n", pilo);
		destroy_mutexes(pilo);
		return (1);
	}
	return (0);
}

int check_dead(t_philo *philo, t_info *info)
{
	int i;
	int b;
	int zbi = 0;

	i = 0;
	while (i < info->nb_philo)
	{
		if (lock(&info->check) == 2)
			return (1);
		b = philo[i].last_eat;
		if (unlock(&info->check) == 2)
			return (1);
		zbi = check_death(b , info->time_to_die, &philo[i]);
		if (zbi)
			return (1);
		i++;
	}
	b = 0;
	zbi = 0;
	lock(&info->check);
	while (b < info->nb_philo)
	{
		if (philo[b].nb_meals <= 0)
		{
			zbi++;
		}
		b++;
	}
	if (zbi == info->nb_philo)
		return (1);
	unlock(&info->check);
	return (0);
}