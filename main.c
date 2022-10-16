/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/16 05:04:44 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_time(int star_time)
{
	struct timeval	tv;
	int			time;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	time = tv.tv_sec * 1000000 + tv.tv_usec;
	return (time/ 1000 - star_time);
}


void ssleep(int time)
{
	int	start;

	start = get_time(0);
	while (get_time(start) < time)
		usleep(100);
}


int	lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) == EINVAL)
		return(2);
	return (1);
}

int	unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) == EINVAL)
		return(2);
	return (1);
}

static int printa(char *str, t_philo *philo)
{
	int check;

	check = 0;

	if	(philo->info)
	{
		lock(&philo->info->check);
		if	(philo->info->end == 1)
		{
			unlock(&philo->info->check);
			return (2);
		}
		unlock(&philo->info->check);
	}
	else
		return (2);
	lock(&philo->info->print);
	printf("%d %d %s", get_time(philo->info->start_time), philo->id, str);
	unlock(&philo->info->print);
	return (1);
}

void	*routine(void *philo)
{
	t_philo *philo_tmp;

	philo_tmp = (t_philo *)philo;
	while (1)
	{
		if (lock(&philo_tmp->left_fork) == 2)
			return (NULL);
		
		printa("phil Take left\n", philo_tmp);

		if (lock(philo_tmp->right_fork) == 2)
			return (NULL);
		printa("phil Take Right\n", philo_tmp);

		lock(&philo_tmp->info->check);
		philo_tmp->last_eat = get_time(0);
		pthread_mutex_unlock(&philo_tmp->info->check);

		printa("phil is eating\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_eat);

		lock(&philo_tmp->info->check);
		philo_tmp->nb_meals--;
		pthread_mutex_unlock(&philo_tmp->info->check);
		
		pthread_mutex_lock(&philo_tmp->info->check);
		if (!philo_tmp->info || philo_tmp->info->end)
			return(NULL);
		pthread_mutex_unlock(&philo_tmp->info->check);
		
		
		if (unlock(philo_tmp->right_fork) == 2)
			return (NULL);
		if (unlock(&philo_tmp->left_fork) == 2)
			return (NULL);

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
		lock(&pilo->info->print);
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
		pthread_mutex_lock(&philo[0].info->check);
		b = philo[i].last_eat;
		pthread_mutex_unlock(&philo[0].info->check);
		zbi = check_death(b , philo[i].info->time_to_die, &philo[i]);
		if (zbi)
		{
			pthread_mutex_lock(&philo[0].info->check);
			philo->info->end = 1;
			// usleep(500000);
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
