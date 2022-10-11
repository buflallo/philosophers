/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:11:18 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/10 19:54:38 by hlachkar         ###   ########.fr       */
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

typedef struct s_info
{
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int start_time;
	pthread_mutex_t *print;
}	t_info;

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	int			is_dead;
	int			last_eat;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
	t_info		info;
}	t_philo;

#endif