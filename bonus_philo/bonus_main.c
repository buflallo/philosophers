/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 03:22:39 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	printa(char *str, t_philo *philo)
{
	lock(philo->info->print);
	printf("%d %d %s", get_time(philo->info->start_time), philo->id, str);
	if (*str == 'd')
		return ;
	unlock(philo->info->print);
}

int	check_meals(t_philo *philo_tmp)
{
	lock(philo_tmp->info->check);
	if (--(philo_tmp->nb_meals) == 0)
	{
		unlock(philo_tmp->info->check);
		unlock(philo_tmp->info->forks);
		unlock(philo_tmp->info->forks);
		return (1);
	}
	unlock(philo_tmp->info->check);
	return (0);
}

void	*routine(void *philo)
{
	t_philo	*philo_tmp;

	philo_tmp = (t_philo *)philo;
	pthread_create(&philo_tmp->thread, NULL, check_dead_child, philo_tmp);
	pthread_detach(philo_tmp->thread);
	while (1)
	{
		lock(philo_tmp->info->forks);
		printa("phil Take left\n", philo_tmp);
		lock(philo_tmp->info->forks);
		printa("phil Take Right\n", philo_tmp);
		printa("phil is eating\n", philo_tmp);
		lock(philo_tmp->is_dead);
		gettimeofday(&philo_tmp->last_eat, NULL);
		unlock(philo_tmp->is_dead);
		ssleep(philo_tmp->info->time_to_eat);
		if (check_meals(philo_tmp))
			return (NULL);
		unlock(philo_tmp->info->forks);
		unlock(philo_tmp->info->forks);
		printa("phil is sleeping\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_sleep);
		printa("phil is thinking\n", philo_tmp);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo		*philos;
	int			i;
	t_info		*info;

	i = 0;
	if (argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (0);
	}
	info = init_info(argv);
	philos = init_philo(info, argv);
	start_philos(info, philos);
	wait_philos(info, philos);
	while (i < info->nb_philo)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
	return (0);
}
