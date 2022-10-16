/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/16 03:48:56 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_philo		philo[atoi(argv[1])];
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
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	info->end = 0;
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
		pthread_mutex_init(&philo[i].fork, NULL);
		philo[i].info = info;
		i++;
	}
	i = 0;
	while (i < info->nb_philo)
	{
		lock(&(info->print));
		philo[i].info->start_time = get_time(0);
		pthread_mutex_unlock(&(info->print));
		if (pthread_create(&philo[i].thread, NULL, routine, (t_philo *)&philo[i]) != 0)
		{
			printf("error");
			return (1);
		}
		pthread_detach(philo[i].thread);
		i++;
	}
	while (!check_dead(philo))
	;
	return (0);
}