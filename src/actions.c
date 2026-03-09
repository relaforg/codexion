/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 15:49:16 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/09 17:39:51 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

int	*take_dongles(t_thread_context *ctx)
{
	int	i;
	int	count;
	int	*dongles;

	i = 0;
	count = 0;
	pthread_mutex_lock(&ctx->pool->mutex);
	while (i < ctx->config->number_of_coder && count < 2)
	{
		if (ctx->pool->dongles[i].is_available == TRUE
			&& now() - ctx->config->start_time - ctx->pool->dongles[i].last_used
			>= ctx->config->cooldown_time)
			count++;
		i++;
	}
	if (count < 2)
	{
		pthread_mutex_unlock(&ctx->pool->mutex);
		return (NULL);
	}
	dongles = malloc(sizeof(int) * 2);
	if (!dongles)
		return (NULL);
	count = 0;
	while (i--)
	{
		if (ctx->pool->dongles[i].is_available == TRUE
			&& now() - ctx->config->start_time - ctx->pool->dongles[i].last_used
			>= ctx->config->cooldown_time)
		{
			ctx->pool->dongles[i].is_available = FALSE;
			dongles[count++] = i;	
		}
	}
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
// void	free_dongles(t_thread_context *ctx)
// {
// 	int	count;
// 	int	i;
//
// 	i = 0;
// 	count = 0;
// 	pthread_mutex_lock(&ctx->pool->mutex);
// 	while (i < ctx->config->number_of_coder && count < 2)
// 	{
// 		if (ctx->pool->dongles[i].is_available == FALSE)
// 		{
// 			ctx->pool->dongles[i].is_available = TRUE;
// 			count++;
// 		}
// 		i++;
// 	}
// 	pthread_mutex_unlock(&ctx->pool->mutex);
// }

void	compile(t_thread_context *ctx)
{
	send_log(ctx->id, COMPILE, ctx->logs);	
	usleep(ctx->config->compilation_time * 1000);
}

void	debug(t_thread_context *ctx)
{
	send_log(ctx->id, DEBUG, ctx->logs);	
	usleep(ctx->config->debug_time * 1000);
}

void	refactor(t_thread_context *ctx)
{
	send_log(ctx->id, REFACTOR, ctx->logs);	
	usleep(ctx->config->refactor_time * 1000);
}
