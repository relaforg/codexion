/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:00:24 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 10:23:04 by relaforg         ###   ########.fr       */
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
		return (FALSE);
	}
	return (TRUE);
}

int	*take_dongles(t_thread_context *ctx)
{
	int	*dongles;
	int	left;
	int	right;

	left = ctx->coder->id % ctx->config->number_of_coder;
	right = (ctx->coder->id + 1) % ctx->config->number_of_coder;
	pthread_mutex_lock(&ctx->pool->mutex);
	if (!dongles_available(ctx, left, right))
	{
		pthread_mutex_unlock(&ctx->pool->mutex);
		return (NULL);
	}
	dongles = malloc(sizeof(int) * 2);
	if (!dongles)
	{
		pthread_mutex_unlock(&ctx->pool->mutex);
		return (NULL);
	}
	ctx->pool->dongles[left].is_available = FALSE;
	ctx->pool->dongles[right].is_available = FALSE;
	pthread_mutex_unlock(&ctx->pool->mutex);
	dongles[0] = left;
	dongles[1] = right;
	print(ctx, DONGLE);
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

t_bool	neighbor_can_compile(t_thread_context *ctx, int neighbor_id)
{
	int		left;
	int		right;
	t_bool	result;

	left = neighbor_id % ctx->config->number_of_coder;
	right = (neighbor_id + 1) % ctx->config->number_of_coder;
	pthread_mutex_lock(&ctx->pool->mutex);
	result = (ctx->pool->dongles[left].is_available
			&& ctx->pool->dongles[right].is_available
			&& now() - ctx->config->start_time
			- ctx->pool->dongles[left].last_used >= ctx->config->cooldown_time
			&& now() - ctx->config->start_time
			- ctx->pool->dongles[right].last_used
			>= ctx->config->cooldown_time);
	pthread_mutex_unlock(&ctx->pool->mutex);
	return (result);
}

int	*ask_dongles(t_thread_context *ctx)
{
	int		i;
	int		neighbor_id;
	int		*dongles;

	if (ctx->config->number_of_coder < 2)
		return (NULL);
	pthread_mutex_lock(&ctx->queue->mutex);
	i = 0;
	while (i < ctx->queue->size && ctx->queue->entries[i].coder->id
		!= ctx->coder->id)
	{
		neighbor_id = ctx->queue->entries[i].coder->id;
		if ((neighbor_id == ((ctx->coder->id - 2 + ctx->config->number_of_coder)
					% ctx->config->number_of_coder) + 1
				|| neighbor_id == (ctx->coder->id
					% ctx->config->number_of_coder) + 1)
			&& neighbor_can_compile(ctx, neighbor_id))
		{
			pthread_mutex_unlock(&ctx->queue->mutex);
			return (NULL);
		}
		i++;
	}
	dongles = take_dongles(ctx);
	pthread_mutex_unlock(&ctx->queue->mutex);
	return (dongles);
}
