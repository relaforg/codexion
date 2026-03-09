/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:33:18 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/09 17:13:54 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

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
	DONE,
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
	long long	start_time;
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

typedef struct s_log_entry
{
	int				coder_id;
	t_message_type	message;
	long long		timestamp;
}	t_log_entry;

typedef struct s_log_queue
{
	t_log_entry		entries[1024];
	int				head;
	int				tail;
	int				count;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_log_queue;

typedef struct s_thread_context
{
	int				id;
	t_config		*config;
	t_dongle_pool	*pool;
	t_log_queue		*logs;
}	t_thread_context;

void		*coder_routine(void *ctx);
void		*monitor_routine(void *ctx);
long long	now(void);
void		refactor(t_thread_context *ctx);
void		debug(t_thread_context *ctx);
void		compile(t_thread_context *ctx);
int			*take_dongles(t_thread_context *ctx);
void		send_log(int id, t_message_type type, t_log_queue *logs);
void		free_dongles(t_thread_context *ctx, int *dongles);

#endif
