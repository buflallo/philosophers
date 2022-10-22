/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 03:40:18 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_info	*init_info(char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	info->nb_philo = atoi(argv[1]);
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	sem_unlink("print");
	sem_unlink("check");
	sem_unlink("forks");
	info->print = sem_open("print", O_CREAT, 0644, 1);
	info->check = sem_open("check", O_CREAT, 0644, 1);
	info->forks = sem_open("forks", O_CREAT, 0644, info->nb_philo);
	return (info);
}

t_philo	*init_philo(t_info *info, char **argv)
{
	t_philo		*philos;
	int			i;
	char		*semi;

	i = 0;
	semi = malloc(2);
	semi[1] = '\0';
	philos = malloc(sizeof(t_philo) * atoi(argv[1]));
	while (i < info->nb_philo)
	{
		philos[i].id = 1 + i;
		philos[i].is_dead = 0;
		philos[i].nb_meals = -1;
		semi[0] = 'a' + i;
		sem_unlink(semi);
		philos[i].is_dead = sem_open(semi, O_CREAT, 0644, 1);
		if (argv[5])
			philos[i].nb_meals = atoi(argv[5]);
		philos[i].info = info;
		i++;
	}
	free(semi);
	return (philos);
}

void	start_philos(t_info *info, t_philo *philos)
{
	int			i;

	i = 0;
	info->start_time = get_time(0);
	while (i < info->nb_philo)
	{
		gettimeofday(&philos[i].last_eat, NULL);
		philos[i].pid = fork();
		if (!philos[i].pid)
		{
			routine(&philos[i]);
			exit(5);
		}
		usleep(100);
		i++;
	}
}

void	wait_philos(t_info *info, t_philo *philos)
{
	int	check;
	int	status;

	check = 0;
	while (check <= info->nb_philo)
	{
		waitpid(0, &status, WUNTRACED);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 5)
				check++;
			else if (WEXITSTATUS(status) == 1)
				break ;
			else
			{
				printf("an error occured\n");
				sems_destroy(info, philos);
				exit(1);
			}
		}
	}
	sems_destroy(info, philos);
}
