/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:33:18 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/07 13:53:22 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>

# define TRUE 1
# define FALSE 0

typedef int	t_bool;
typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef enum e_message_type
{
	DONGLE,
	COMPILE,
	DEBUG,
	REFACTOR,
	BURNOUT,
	NONE
}	t_message_type;

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

typedef struct s_message
{
	int	coder_id;
	t_message_type	type;
	pthread_mutex_t mutex;
	pthread_cond_t	cond;
}	t_message;

typedef struct s_thread_context
{
	int				id;
	t_config		*config;
	t_dongle_pool	*pool;
	t_message		*message;
}	t_thread_context;


void	*coder_routine(void *ctx);

#endif
