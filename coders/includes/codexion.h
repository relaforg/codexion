/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:33:18 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 16:48:10 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# define _DEFAULT_SOURCE
# define _XOPEN_SOURCE 500

# include <pthread.h>
# include <sys/time.h>

# define TRUE 1
# define FALSE 0

typedef int	t_bool;
typedef enum e_scheduler
{
	FIFO,
	EDF,
	NONE
}	t_scheduler;

typedef enum e_message_type
{
	DONGLE,
	COMPILE,
	DEBUG,
	REFACTOR,
	BURNOUT,
	DONE,
	SHUTDOWN
}	t_message_type;

typedef struct s_config
{
	int			number_of_coder;
	int			burnout_time;
	int			compilation_time;
	int			debug_time;
	int			refactor_time;
	int			number_of_compilation;
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

typedef struct s_print
{
	pthread_mutex_t	mutex;
}	t_print;

typedef struct s_burnout
{
	t_bool			is_burnout;
	pthread_mutex_t	mutex;
}	t_burnout;

typedef struct s_coder
{
	int				id;
	pthread_t		tid;
	int				last_compile;
	int				nbr_compilation;
	pthread_mutex_t	mutex;
}	t_coder;

typedef struct s_queue_entry
{
	t_coder		*coder;
	long long	request_time;
	long long	deadline;
}	t_queue_entry;

typedef struct s_scheduler_queue
{
	t_queue_entry	*entries;
	int				size;
	pthread_mutex_t	mutex;
	void			(*sort)(struct s_scheduler_queue*);
}	t_scheduler_queue;

typedef struct s_thread_context
{
	t_coder				*coder;
	t_config			*config;
	t_dongle_pool		*pool;
	t_print				*print;
	t_scheduler_queue	*queue;
	t_burnout			*burnout;
}	t_thread_context;

typedef struct s_env
{
	t_config			config;
	t_dongle_pool		pool;
	t_print				print;
	t_scheduler_queue	queue;
	pthread_t			monitor;
	t_burnout			burnout;
	int					nb_coders_launched;
	t_coder				*coders;
}	t_env;

void		*coder_routine(void *ctx);
void		*monitor_routine(void *ctx);
long long	now(void);
void		refactor(t_thread_context *ctx);
void		debug(t_thread_context *ctx);
void		compile(t_thread_context *ctx);
int			*take_dongles(t_thread_context *ctx);
void		free_dongles(t_thread_context *ctx, int *dongles);
void		fifo_sort(t_scheduler_queue *queue);
void		edf_sort(t_scheduler_queue *queue);
int			validate_args(int argc, char **argv, t_config *config);
int			*ask_dongles(t_thread_context *ctx);
int			init_dongle_pool(t_dongle_pool *pool, t_config *config);
int			init_coders(t_coder **coders, t_config *config);
int			init_queue(t_scheduler_queue *queue, t_config *config,
				t_coder *coders);
int			init_env(t_env *env, int argc, char **argv);
void		clean_env(t_env *env);
void		fill_context(t_thread_context *ctx, t_env *env);
void		update_queue(t_thread_context *ctx);
void		pop_queue(t_thread_context *ctx);
void		print(t_thread_context *ctx, t_message_type type);

#endif
