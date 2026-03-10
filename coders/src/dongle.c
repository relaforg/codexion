/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:00:24 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 16:37:44 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

t_bool	dongles_available(t_thread_context *ctx, int left, int right)
{
	if (!ctx->pool->dongles[left].is_available
		|| !ctx->pool->dongles[right].is_available
		|| now() - ctx->config->start_time - ctx->pool->dongles[left].last_used
		< ctx->config->cooldown_time
		|| now() - ctx->config->start_time - ctx->pool->dongles[right].last_used
		< ctx->config->cooldown_time)
	{
		pthread_mutex_unlock(&ctx->pool->mutex);
		return (FALSE);
	}
	return (TRUE);
}

int	*take_dongles(t_thread_context *ctx)
{
	int	*dongles;
	int	left;
	int	right;

	left = ctx->id % ctx->config->number_of_coder;
	right = (ctx->id + 1) % ctx->config->number_of_coder;
	pthread_mutex_lock(&ctx->pool->mutex);
	if (!dongles_available(ctx, left, right))
		return (NULL);
	dongles = malloc(sizeof(int) * 2);
	if (!dongles)
	{
		pthread_mutex_unlock(&ctx->pool->mutex);
		return (NULL);
	}
	ctx->pool->dongles[left].is_available = FALSE;
	ctx->pool->dongles[right].is_available = FALSE;
	dongles[0] = left;
	dongles[1] = right;
	pthread_mutex_unlock(&ctx->pool->mutex);
	send_log(ctx->id, DONGLE, ctx->logs);
	send_log(ctx->id, DONGLE, ctx->logs);
	return (dongles);
}

void	free_dongles(t_thread_context *ctx, int *dongles)
{
	long long	release_time;

	release_time = now() - ctx->config->start_time;
	pthread_mutex_lock(&ctx->pool->mutex);
	ctx->pool->dongles[dongles[0]].is_available = TRUE;
	ctx->pool->dongles[dongles[0]].last_used = release_time;
	ctx->pool->dongles[dongles[1]].is_available = TRUE;
	ctx->pool->dongles[dongles[1]].last_used = release_time;
	pthread_mutex_unlock(&ctx->pool->mutex);
	free(dongles);
}

void	ensure_in_queue(t_thread_context *ctx, long long last_compile)
{
	int		i;

	i = 0;
	while (i < ctx->queue->size)
	{
		if (ctx->queue->entries[i].coder_id == ctx->id)
			break ;
		i++;
	}
	if (i == ctx->queue->size)
	{
		ctx->queue->entries[ctx->queue->size].coder_id = ctx->id;
		ctx->queue->entries[ctx->queue->size].request_time = now();
		ctx->queue->entries[ctx->queue->size].deadline = last_compile
			+ ctx->config->burnout_time;
		ctx->queue->size++;
	}
}

t_bool	ask_dongles(t_thread_context *ctx, long long last_compile)
{
	t_bool	result;

	pthread_mutex_lock(&ctx->queue->mutex);
	ensure_in_queue(ctx, last_compile);
	ctx->queue->sort(ctx->queue);
	if (ctx->queue->entries[ctx->queue->size - 1].coder_id == ctx->id)
		result = TRUE;
	else
		result = FALSE;
	pthread_mutex_unlock(&ctx->queue->mutex);
	return (result);
}
