/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/22 03:14:45 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_info	*init_info(char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	info->nb_philo = atoi(argv[1]);
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	if (pthread_mutex_init(&info->print, NULL) || \
	pthread_mutex_init(&info->check, NULL))
		return (NULL);
	return (info);
}

t_philo	*init_philo(t_info *info, char **argv)
{
	t_philo		*philos;
	int			i;

	i = 0;
	philos = malloc(sizeof(t_philo) * atoi(argv[1]));
	while (i < info->nb_philo)
	{
		philos[i].id = 1 + i;
		philos[i].is_dead = 0;
		philos[i].nb_meals = -11;
		if (argv[5] && atoi(argv[5]) >= 0)
			philos[i].nb_meals = atoi(argv[5]);
		pthread_mutex_init(&philos[i].fork, NULL);
		philos[i].info = info;
		i++;
	}
	return (philos);
}

int	run_threads(t_info *info, t_philo *philos)
{
	int			i;

	i = 0;
	info->start_time = get_time(0);
	while (i < info->nb_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, \
		routine, (t_philo *)&philos[i]) != 0)
		{
			printf("error");
			return (1);
		}
		pthread_detach(philos[i].thread);
		usleep(100);
		i++;
	}
	return (0);
}

int	check_dead(t_philo *philo, t_info *info)
{
	int	i;
	int	b;

	i = 0;
	while (i < info->nb_philo)
	{
		b = philo[i].last_eat;
		b = check_death(b, info->time_to_die, &philo[i]);
		if (b)
			return (1);
		i++;
	}
	return (check_meals(philo, info));
}

int	main(int argc, char **argv)
{
	t_philo		*philo;
	int			i;
	t_info		*info;

	i = 0;
	if (argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (0);
	}
	info = init_info(argv);
	if (!info)
		return (1);
	philo = init_philo(info, argv);
	if (!philo)
		return (1);
	if (run_threads(info, philo))
		return (1);
	while (!check_dead(philo, info))
		;
	return (0);
}
