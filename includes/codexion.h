/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:33:18 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/07 09:50:53 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>

# define TRUE 1
# define FALSE 0

typedef int	t_bool;
typedef enum	e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coder;
	int			number_of_compilation;
	int			compilation_time;
	int			debug_time;
	int			refactor_time;
	int			burnout_time;
	int			cooldown_time;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_dongle
{
	t_bool	is_available;
	int		last_used;
}	t_dongle;

typedef struct s_dongle_pool
{
	t_dongle		*dongles;
	int				size;
	pthread_mutex_t	mutex;	
}	t_dongle_pool;

#endif
