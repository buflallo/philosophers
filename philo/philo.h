/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:11:18 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/21 23:51:56 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <errno.h>

typedef struct s_info
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				start_time;
	int				nb_philo;
	pthread_mutex_t	print;
	pthread_mutex_t	check;
}	t_info;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				is_dead;
	int				last_eat;
	int				nb_meals;
	pthread_mutex_t	fork;
	t_info			*info;
}	t_philo;

int		get_time(int star_time);
int		check_dead(t_philo *philo, t_info *info);
void	*routine(void *arg);
void	ssleep(int time);
int		lock(pthread_mutex_t *mutex);
int		printa(char *str, t_philo *philo);
int		unlock(pthread_mutex_t *mutex);
int		check_meals(t_philo *philo, t_info *info);
int		check_death(int l_eat, int t_die, t_philo *pilo);
int		check_meals(t_philo *philo, t_info *info);
int		complete_routine(t_philo *philo_tmp);

#endif