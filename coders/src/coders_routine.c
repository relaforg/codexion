/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:34:27 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 10:32:09 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h" 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	check_burnout(t_thread_context *ctx)
{
	pthread_mutex_lock(&ctx->burnout->mutex);
	if (ctx->burnout->is_burnout)
	{
		pthread_mutex_unlock(&ctx->burnout->mutex);
		return (1);
	}
	pthread_mutex_unlock(&ctx->burnout->mutex);
	return (0);
}

void	print(t_thread_context *ctx, t_message_type type)
{
	long long	t;

	t = now();
	if (check_burnout(ctx))
		return ;
	pthread_mutex_lock(&ctx->print->mutex);
	printf("%lld %d ", t - ctx->config->start_time, ctx->coder->id);
	if (type == DONGLE)
	{
		printf("has taken a dongle\n");
		printf("%lld %d ", t - ctx->config->start_time, ctx->coder->id);
		printf("has taken a dongle\n");
	}
	else if (type == COMPILE)
		printf("is compiling\n");
	else if (type == DEBUG)
		printf("is debugging\n");
	else if (type == REFACTOR)
		printf("is refactoring\n");
	pthread_mutex_unlock(&ctx->print->mutex);
}

void	constant_coder_routine(t_thread_context *ctx, int *dongles)
{
	ctx->coder->last_compile = now() - ctx->config->start_time;
	update_queue(ctx);
	compile(ctx);
	free_dongles(ctx, dongles);
	debug(ctx);
	refactor(ctx);
	ctx->coder->nbr_compilation++;
}

void	*coder_routine(void *context)
{
	t_thread_context	*ctx;
	int					*dongles;

	ctx = (t_thread_context *) context;
	while (ctx->coder->nbr_compilation < ctx->config->number_of_compilation)
	{
		if (check_burnout(ctx))
		{
			free(context);
			return (0);
		}
		dongles = ask_dongles(ctx);
		if (!dongles)
			continue ;
		constant_coder_routine(ctx, dongles);
	}
	pop_queue(ctx);
	free(context);
	return (NULL);
}
