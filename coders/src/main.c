/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:38:50 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 15:34:04 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

long long	now(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000L + tv.tv_usec) / 1000);
}

int	launch_monitor(t_env *env)
{
	t_thread_context	*ctx;

	ctx = malloc(sizeof(t_thread_context));
	if (!ctx)
		return (1);
	ctx->id = -1;
	ctx->config = &env->config;
	ctx->pool = &env->pool;
	ctx->logs = &env->logs;
	ctx->queue = &env->queue;
	if (pthread_create(&env->monitor, NULL, monitor_routine, (void *)ctx))
	{
		free(ctx);
		return (1);
	}
	return (0);
}

int	launch_coders(pthread_t *coders, t_env *env)
{
	t_thread_context	*ctx;
	int					i;

	i = 0;
	while (i < env->config.number_of_coder)
	{
		ctx = malloc(sizeof(t_thread_context));
		if (!ctx)
			break ;
		ctx->id = i + 1;
		ctx->config = &env->config;
		ctx->pool = &env->pool;
		ctx->logs = &env->logs;
		ctx->queue = &env->queue;
		if (pthread_create(&coders[i], NULL, coder_routine, (void *)ctx))
		{
			free(ctx);
			free(env->pool.dongles);
			return (1);
		}
		i++;
	}
	return (0);
}

void	join_threads(t_env *env, pthread_t *coders)
{
	int	i;

	i = 0;
	while (i < env->config.number_of_coder)
	{
		pthread_join(coders[i], NULL);
		i++;
	}
	pthread_join(env->monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_env		env;
	pthread_t	*coders;

	if (init_env(&env, argc, argv))
		return (1);
	coders = malloc(sizeof(pthread_t) * env.config.number_of_coder);
	if (!coders)
		return (1);
	if (launch_monitor(&env))
	{
		free(coders);
		return (1);
	}
	if (launch_coders(coders, &env))
		return (1);
	join_threads(&env, coders);
	free(env.pool.dongles);
	free(coders);
	free(env.queue.entries);
	return (0);
}
