/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:34:27 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 15:34:11 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h" 

void	send_log(int id, t_message_type type, t_log_queue *logs)
{
	pthread_mutex_lock(&logs->mutex);
	logs->count++;
	logs->entries[logs->tail].coder_id = id;
	logs->entries[logs->tail].message = type;
	logs->entries[logs->tail].timestamp = now();
	logs->tail = (logs->tail + 1) % 1024;
	pthread_cond_broadcast(&logs->cond);
	pthread_mutex_unlock(&logs->mutex);
}

void	pop_queue_tail(t_scheduler_queue *queue)
{
	pthread_mutex_lock(&queue->mutex);
	queue->size--;
	pthread_mutex_unlock(&queue->mutex);
}

int	check_burnout(t_thread_context *ctx, long long last_compile)
{
	if (now() - ctx->config->start_time - last_compile
		>= ctx->config->burnout_time)
	{
		send_log(ctx->id, BURNOUT, ctx->logs);
		return (1);
	}
	return (0);
}

void	constant_coder_routine(t_thread_context *ctx, long long *last_compile,
							int	*dongles, int *nbr_compilation)
{
	pop_queue_tail(ctx->queue);
	*last_compile = now() - ctx->config->start_time;
	compile(ctx);
	free_dongles(ctx, dongles);
	debug(ctx);
	refactor(ctx);
	*(nbr_compilation) = *(nbr_compilation) + 1;
}

void	*coder_routine(void *context)
{
	t_thread_context	*ctx;
	int					nbr_compilation;
	long long			last_compile;
	int					*dongles;

	ctx = (t_thread_context *) context;
	nbr_compilation = 0;
	last_compile = 0;
	while (nbr_compilation < ctx->config->number_of_compilation)
	{
		if (check_burnout(ctx, last_compile))
		{
			free(context);
			return (NULL);
		}
		if (!ask_dongles(ctx, last_compile))
			continue ;
		dongles = take_dongles(ctx);
		if (!dongles)
			continue ;
		constant_coder_routine(ctx, &last_compile, dongles, &nbr_compilation);
	}
	send_log(ctx->id, DONE, ctx->logs);
	free(context);
	return (NULL);
}
