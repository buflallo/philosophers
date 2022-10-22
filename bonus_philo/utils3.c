/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 03:41:48 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sems_destroy(t_info *info, t_philo *philos)
{
	int		i;
	char	*name;
	char	*semi;

	i = 0;
	semi = malloc(2);
	semi[1] = '\0';
	sem_close(info->print);
	sem_close(info->forks);
	sem_close(info->check);
	sem_unlink("print");
	sem_unlink("forks");
	sem_unlink("check");
	semi[0] = 'a';
	while (i < info->nb_philo)
	{
		sem_close(philos[i].is_dead);
		semi[0] += i;
		sem_unlink(name);
		i++;
	}
}

int	check_death(int l_eat, int t_die, t_philo *pilo)
{
	if (get_time(0) - l_eat >= t_die)
	{
		printa("died\n", pilo);
		return (1);
	}
	return (0);
}

void	*check_dead_child(void *phila)
{
	int		i;
	int		b;
	int		zbi;
	t_philo	*philo;

	zbi = 0;
	philo = (t_philo *)phila;
	while (1)
	{
		i = 0;
		lock(philo->is_dead);
		b = convert_time(philo->last_eat) / 1000;
		b = check_death(b, philo->info->time_to_die, philo);
		unlock(philo->is_dead);
		if (b)
			exit(1);
	}
	return (NULL);
}
