/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/19 03:27:22 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_philo		*philo;
	int			i;
	t_info *info;
	
	i = 0;
	
	if(argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (0);
	}
	info = malloc(sizeof(t_info));
	info->nb_philo = atoi(argv[1]);
	philo = malloc(sizeof(t_philo) * info->nb_philo);
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	if (pthread_mutex_init(&info->print, NULL) || pthread_mutex_init(&info->check, NULL))
		return (1);
	while (i < info->nb_philo)
	{
		philo[i].id = 1+i;
		philo[i].is_dead = 0;
		philo[i].nb_meals = -1;;
		if (argv[5])
			philo[i].nb_meals = atoi(argv[5]);
		philo[i].last_eat = get_time(0);
		pthread_mutex_init(&philo[i].left_fork, NULL);
		if (philo[i].id == info->nb_philo)
			philo[i].right_fork = &philo[0].left_fork;
		else
		{
			philo[i].right_fork = &philo[i+1].left_fork;
		}
		philo[i].info = info;
		i++;
	}
	i = 0;
	while (i < info->nb_philo)
	{
		lock(&(info->print));
		info->start_time = get_time(0);
		pthread_mutex_unlock(&(info->print));
		if (pthread_create(&philo[i].thread, NULL, routine, (t_philo *)&philo[i]) != 0)
		{
			printf("error");
			return (1);
		}
		pthread_detach(philo[i].thread);
		i++;
	}
	while (!check_dead(philo, info))
	;
	return (0);
}
