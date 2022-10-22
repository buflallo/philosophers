/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 23:52:33 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *philo)
{
	t_philo	*philo_tmp;

	philo_tmp = (t_philo *)philo;
	philo_tmp->last_eat = get_time(0);
	while (1)
	{
		if (lock(&philo_tmp->fork))
			return (NULL);
		if (printa("phil Take left\n", philo_tmp) == 2)
			return (NULL);
		if (philo_tmp->id == philo_tmp->info->nb_philo)
			lock(&(philo_tmp - philo_tmp->info->nb_philo + 1)->fork);
		else
			lock(&(philo_tmp + 1)->fork);
		if (printa("phil Take Right\n", philo_tmp) == 2)
			return (NULL);
		if (lock(&philo_tmp->info->check))
			return (NULL);
		philo_tmp->last_eat = get_time(0);
		if (unlock(&philo_tmp->info->check))
			return (NULL);
		if (complete_routine(philo_tmp))
			return (NULL);
	}
	return (NULL);
}
