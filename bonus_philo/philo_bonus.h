/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:11:18 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 04:04:16 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <errno.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_info
{
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		start_time;
	int		nb_philo;
	sem_t	*print;
	sem_t	*check;
	sem_t	*forks;
}	t_info;

typedef struct s_philo
{
	int				id;
	int				pid;
	pthread_t		thread;
	sem_t			*is_dead;
	struct timeval	last_eat;
	int				nb_meals;
	t_info			*info;
}	t_philo;

int		get_time(int star_time);
void	ssleep(int time);
void	lock(sem_t	*sem);
void	unlock(sem_t	*sem);
int		convert_time(struct timeval tv);

t_info	*init_info(char **argv);
t_philo	*init_philo(t_info *info, char **argv);
void	start_philos(t_info *info, t_philo *philos);
void	wait_philos(t_info *info, t_philo *philos);

void	sems_destroy(t_info *info, t_philo *philos);
int		check_death(int l_eat, int t_die, t_philo *pilo);
void	*check_dead_child(void *phila);
void	*routine(void *philo);
void	printa(char *str, t_philo *philo);

#endif