/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 17:15:30 by hlachkar          #+#    #+#             */
/*   Updated: 2022/10/18 22:44:22 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
