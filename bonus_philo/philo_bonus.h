/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:11:18 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/20 01:22:23 by hlachkar         ###   ########.fr       */
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
# include <stdatomic.h>

typedef struct s_info
{
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int start_time;
	int end;
	int nb_philo;
	sem_t *print;
	sem_t *check;
	sem_t	*forks;
}	t_info;

typedef struct s_philo
{
	int			id;
	int			pid;
	pthread_t	thread;
	int			is_dead;
	int			last_eat;
	int			nb_meals;
	t_info		*info;
}	t_philo;

int		get_time(int star_time);
void 	ssleep(int time);
#endif