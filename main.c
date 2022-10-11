/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/10 20:37:52 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time(int star_time)
{
	struct timeval	tv;
	int			time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec*1000000 + tv.tv_usec;
	return (time / 1000 - star_time);
}

void	*routine(void *philo)
{
	t_philo *philo_tmp;

	philo_tmp = (t_philo *)philo;
	while (!philo_tmp->is_dead)
	{
		if (get_time(philo_tmp->info.start_time) - philo_tmp->last_eat > philo_tmp->info.time_to_die)
			philo_tmp->is_dead = 1;
		pthread_mutex_lock(philo_tmp->right_fork);
		pthread_mutex_lock(&philo_tmp->left_fork);
		
		pthread_mutex_lock(philo_tmp->info.print);
		printf("%d %d philo is eating\n", get_time(philo_tmp->info.start_time), philo_tmp->id);
		pthread_mutex_unlock(philo_tmp->info.print);
		
		philo_tmp->last_eat = get_time(philo_tmp->info.start_time);
		usleep(philo_tmp->info.time_to_eat * 1000);

		pthread_mutex_unlock(philo_tmp->right_fork);
		pthread_mutex_unlock(&philo_tmp->left_fork);

		pthread_mutex_lock(philo_tmp->info.print);
		printf("%d %d philo sleeping\n", get_time(philo_tmp->info.start_time), philo_tmp->id);
		pthread_mutex_unlock(philo_tmp->info.print);
		
		usleep(philo_tmp->info.time_to_eat * 1000);
		
	}
	pthread_exit(&philo_tmp->id);
}

int	main(void)
{
	t_philo		philo[4];
	char		*arg;
	void		*res;
	int			i;

	i = 0;
	arg = strdup("9lwa");
	while (i < 4)
	{
		philo[i].info.time_to_die = 100;
		philo[i].info.time_to_eat = 100;
		philo[i].info.time_to_sleep = 100;
		philo[i].id = 1+i;
		philo[i].is_dead = 0;
		philo[i].last_eat = 0;
		pthread_mutex_init(&philo[i].left_fork, NULL);
		if (philo[i].id == 4)
			philo[i].right_fork = &philo[0].left_fork;
		else
		{
			philo[i].right_fork = &philo[i+1].left_fork;
		}
		if (i == 0)
		{
			philo[0].info.print = malloc(sizeof(pthread_mutex_t));
			if (pthread_mutex_init(philo[0].info.print, NULL))
				return (1);
		}
		else
			philo[i].info.print = philo[0].info.print;
		i++;
	}
	i = 0;
	while (i < 4)
	{
		philo[i].info.start_time = get_time(0);
		if (pthread_create(&philo[i].thread, NULL, routine, (t_philo *)&philo[i]) != 0)
		{
			printf("error");
			return (1);
		}
		i++;
		usleep(40);
	}
	i=0;
	while (!philo[0].is_dead);
	return (0);
}
