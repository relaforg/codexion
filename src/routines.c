/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:34:27 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 11:37:03 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h" 
#include <pthread.h>
#include <stdio.h>

long long	now(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000L + tv.tv_usec) / 1000);
}

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


t_bool	ask_dongles(t_thread_context *ctx, long long last_compile)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&ctx->queue->mutex);
	while (i < ctx->queue->size)
	{
		if (ctx->queue->entries[i].coder_id == ctx->id)
			break;
		i++;
	}
	if (i == ctx->queue->size)
	{
		ctx->queue->entries[ctx->queue->size].coder_id = ctx->id;
		ctx->queue->entries[ctx->queue->size].request_time = now();
		ctx->queue->entries[ctx->queue->size].deadline = last_compile +
													ctx->config->burnout_time;
	}
	pthread_mutex_unlock(&ctx->queue->mutex);
	return (TRUE);
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
		if (now() - ctx->config->start_time - last_compile
			>= ctx->config->burnout_time)
		{
			send_log(ctx->id, BURNOUT, ctx->logs);
			free(context);
			return (NULL);
		}
		if (!ask_dongles(ctx))
			continue ;
		dongles = take_dongles(ctx);
		if (!dongles)
			continue ;
		compile(ctx);
		free_dongles(ctx, dongles);
		debug(ctx);
		refactor(ctx);
		nbr_compilation++;
	}
	send_log(ctx->id, DONE, ctx->logs);
	free(context);
	return (NULL);
}

void	*monitor_routine(void *context)
{
	t_thread_context	*ctx;
	int					count;
	t_log_entry			msg;

	ctx = (t_thread_context *) context;
	count = 0;
	while (count < ctx->config->number_of_coder)
	{
		pthread_mutex_lock(&ctx->logs->mutex);
		while (ctx->logs->count == 0)
			pthread_cond_wait(&ctx->logs->cond, &ctx->logs->mutex);
		msg = ctx->logs->entries[ctx->logs->head];
		ctx->logs->head = (ctx->logs->head + 1) % 1024;
		ctx->logs->count--;
		pthread_mutex_unlock(&ctx->logs->mutex);
		if (msg.message == DONE)
		{
			count++;
			continue;
		}
		printf("%lld %d ", msg.timestamp - ctx->config->start_time,
				msg.coder_id);
		if (msg.message == DONGLE)
			printf("has taken a dongle\n");
		else if (msg.message == COMPILE)
			printf("is compiling\n");
		else if (msg.message == DEBUG)
			printf("is debugging\n");
		else if (msg.message == REFACTOR)
			printf("is refactoring\n");
		else if (msg.message == BURNOUT)
		{
			printf("burned out\n");
			free(context);
			return (NULL);
		}
	}
	printf("\nAll coders are done !\n");
	free(context);
	return (NULL);
}
