/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/16 02:51:59 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock(pthread_mutex_t *mutex, t_info *info)
{
	if(info)
	{
		if (info->end)
			pthread_exit(NULL);
	}
	else
		pthread_exit(NULL);
	pthread_mutex_lock(mutex);
	return (1);
}

static int printa(char *str, t_philo *philo)
{
	int check;

	check = 0;

	if	(philo->info)
	{
		lock(&philo->info->check, philo->info);
		if	(philo->info->end == 1)
		{
			pthread_mutex_unlock(&philo->info->check);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&philo->info->check);
	}
	else
		pthread_exit(NULL);
	lock(&philo->info->print, philo->info);
	printf("%d %d %s", get_time(philo->info->start_time), philo->id, str);
	pthread_mutex_unlock(&philo->info->print);
	return (1);
}

void	*routine(void *philo)
{
	t_philo *philo_tmp;

	philo_tmp = (t_philo *)philo;
	while (1)
	{
		lock(&philo_tmp->left_fork, philo_tmp->info);
		printa("phil Take left\n", philo_tmp);

		lock(philo_tmp->right_fork, philo_tmp->info);
		printa("phil Take Right\n", philo_tmp);
		
		lock(&philo_tmp->info->check, philo_tmp->info);
		philo_tmp->last_eat = get_time(0);
		pthread_mutex_unlock(&philo_tmp->info->check);
		
		printa("phil is eating\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_eat);
		
		lock(&philo_tmp->info->check, philo_tmp->info);
		philo_tmp->nb_meals--;
		pthread_mutex_unlock(&philo_tmp->info->check);
		
		pthread_mutex_unlock(philo_tmp->right_fork);
		pthread_mutex_unlock(&philo_tmp->left_fork);
		
		printa("phil is sleeping\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_sleep);
			
		printa("phil is thinking\n", philo_tmp);
	}
	return (NULL);
}

static int	check_death(int l_eat, int t_die, t_philo *pilo)
{
	if (get_time(0) - l_eat >= t_die)
	{
		lock(&pilo->info->print, pilo->info);
		printf("%d %d died\n", get_time(pilo->info->start_time),(int)pilo->id);
		pthread_mutex_unlock(&pilo->info->print);
		return (1);
	}
	return (0);
}

int check_dead(t_philo *philo)
{
	int i;
	int b;
	int zbi = 0;

	i = 0;
	while (i < philo[0].info->nb_philo)
	{
		lock(&philo[0].info->check, philo[0].info);
		b = philo[i].last_eat;
		pthread_mutex_unlock(&philo[0].info->check);
		zbi = check_death(b , philo[i].info->time_to_die, &philo[i]);
		if (zbi)
		{
			lock(&philo[0].info->check, philo[0].info);
			philo->info->end = 1;
			pthread_mutex_unlock(&philo[0].info->check);
			return (1);
		}
		// b = 0;
		// zbi = 0;
		// lock(&philo[0].info->check);
		// while (b < philo[0].info->nb_philo)
		// {
		// 	if (philo[b].nb_meals == 0)
		// 		zbi++;
		// 	b++;
		// }
		// pthread_mutex_unlock(&philo[0].info->check);
		// if (zbi == philo[0].info->nb_philo)
		// {
		// 	lock(&philo[0].info->check);
		// 	philo->info->end = 1;
		// 	pthread_mutex_unlock(&philo[0].info->check);
		// 	return (1);
		// }
		i++;
	}
	return (0);
}
