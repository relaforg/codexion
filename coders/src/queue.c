/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:16:48 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 11:28:55 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	pop_queue(t_thread_context *ctx)
{
	int	i;

	pthread_mutex_lock(&ctx->queue->mutex);
	i = 0;
	while (i < ctx->queue->size && ctx->queue->entries[i].coder->id
		!= ctx->coder->id)
		i++;
	while (i < ctx->queue->size - 1)
	{
		ctx->queue->entries[i] = ctx->queue->entries[i + 1];
		i++;
	}
	ctx->queue->size--;
	pthread_mutex_unlock(&ctx->queue->mutex);
}

void	update_queue(t_thread_context *ctx)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&ctx->queue->mutex);
	while (i < ctx->queue->size)
	{
		if (ctx->queue->entries[i].coder->id == ctx->coder->id)
		{
			ctx->queue->entries[i].deadline = (now() - ctx->config->start_time)
				+ ctx->config->burnout_time;
			ctx->queue->entries[i].request_time = now();
			break ;
		}
		i++;
	}
	ctx->queue->sort(ctx->queue);
	pthread_mutex_unlock(&ctx->queue->mutex);
}
