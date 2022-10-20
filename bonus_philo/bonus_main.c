/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/20 01:49:35 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	lock(sem_t	*sem)
{
	if (sem_wait(sem))
		return(printf("sem wait failed\n"));
	return (0);
}


int	unlock(sem_t	*sem)
{
	if (sem_post(sem))
		return(printf("sem wait failed\n"));
	return (0);
}

static int printa(char *str, t_philo *philo)
{
	if (lock(philo->info->print) == 2)
		return(2);
	printf("%d %d %s", get_time(philo->info->start_time), philo->id, str);
	if (*str == 'd')
		return (2);
	if(unlock(philo->info->print) == 2) 
		return (2);
	return (1);
}

static int	check_death(int l_eat, int t_die, t_philo *pilo)
{
	if (get_time(0) - l_eat >= t_die)
	{
		// lock(pilo->info->print);
		printa("died\n", pilo);
		// destroy_sems(pilo);
		return (1);
	}
	return (0);
}

void *check_dead_child(void *phila)
{
	int i;
	int b;
	int zbi = 0;
	t_philo *philo;

	philo = (t_philo *)phila;
	while(1)
	{
		i = 0;
		if (lock(philo->info->check) == 2)
			return (NULL);
		if (check_death(philo->last_eat , philo->info->time_to_die, philo))
			exit(1);
		if (unlock(philo->info->check) == 2)
			return (NULL);
	}
	return (NULL);
}

void	*routine(void *philo)
{
	t_philo *philo_tmp;

	philo_tmp = (t_philo *)philo;
	
	// printf("id== %d\n",philo_tmp->id);
	pthread_create(&philo_tmp->thread, NULL, &check_dead_child, philo_tmp);
	pthread_detach(philo_tmp->thread);
	while (1)
	{
		if (lock(philo_tmp->info->forks))
			return (NULL);

		if (printa("phil Take left\n", philo_tmp) == 2)
			return (NULL);

		if (lock(philo_tmp->info->forks))
			return (NULL);
		if (printa("phil Take Right\n", philo_tmp) == 2)
			return (NULL);

		if (lock(philo_tmp->info->check))
			return (NULL);
		philo_tmp->last_eat = get_time(0);
		if (unlock(philo_tmp->info->check))
			return (NULL);

		printa("phil is eating\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_eat);

		if (lock(philo_tmp->info->check))
			return (NULL);
		if (--(philo_tmp->nb_meals) <= 0)
		{
			printf("phil %d is done eating\n", philo_tmp->id);
			if (unlock(philo_tmp->info->check))
				return (NULL);
			if (unlock(philo_tmp->info->forks))
				return (NULL);
			if (unlock(philo_tmp->info->forks))
				return (NULL);
			return (NULL);
		}
		if (unlock(philo_tmp->info->check))
			return (NULL);
		
		if (unlock(philo_tmp->info->forks))
			return (NULL);
		if (unlock(philo_tmp->info->forks))
			return (NULL);

		printa("phil is sleeping\n", philo_tmp);
		ssleep(philo_tmp->info->time_to_sleep);

		printa("phil is thinking\n", philo_tmp);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo		philo[atoi(argv[1])];
	int			i;
	t_info *info;
	int status;
	int check = 0;
	
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
	sem_unlink("print");
	sem_unlink("check");
	sem_unlink("forks");
	info->print = sem_open("print", O_CREAT, 0644, 1);
	info->check = sem_open("check", O_CREAT, 0644, 1);
	info->forks = sem_open("forks", O_CREAT, 0644, info->nb_philo);
	while (i < info->nb_philo)
	{
		philo[i].id = 1+i;
		philo[i].is_dead = 0;
		philo[i].nb_meals = -1;
		if (argv[5])
			philo[i].nb_meals = atoi(argv[5]);
		philo[i].last_eat = get_time(0);
		philo[i].info = info;
		i++;
	}
	i = 0;
	int k = 0;
	info->start_time = get_time(0);
	while (i < info->nb_philo)
	{
		philo[i].last_eat = get_time(0);
		// printf("id== %d\n",philo[i].id);
		philo[i].pid = fork();
		if (!philo[i].pid)
		{
			// printf("id== %d\n",philo[i].id);
			routine(&philo[i]);
			exit(5);
		}
		usleep(100);
		i++;
	}
	
	i = 0;
	while (i < info->nb_philo && check <= info->nb_philo)
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 5)
			{
				check++;
			}	
			if (WEXITSTATUS(status) == 1)
				break;
		}
		i++;
	}
	i = 0;
	while (i < info->nb_philo)
	{
		kill(philo[i].pid, SIGKILL);
		i++;
	}
	return (0);
}