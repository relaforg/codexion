/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:38:50 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/09 16:04:41 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>

int	init_dongle_pool(t_dongle_pool *pool, t_config *config)
{
	int	i;

	pool->dongles = malloc(sizeof(t_dongle) * config->number_of_coder);
	if (!pool->dongles)
		return (1);
	pool->size = config->number_of_coder;
	i = 0;
	while (i < config->number_of_coder)
	{
		pool->dongles[i].is_available = TRUE;
		pool->dongles[i].last_used = -config->cooldown_time;
		i++;
	}
	if (pthread_mutex_init(&pool->mutex, NULL))
	{
		free(pool->dongles);
		return (1);
	}
	return (0);
}

int	init_logs(t_log_queue *logs)
{
	logs->head = 0;
	logs->tail = 0;
	logs->count = 0;
	if (pthread_mutex_init(&logs->mutex, NULL))
		return (1);
	if (pthread_cond_init(&logs->cond, NULL))
		return (1);
	return (0);
}

int	main(void)
{
	pthread_t	*coders;
	pthread_t	tmp;
	t_config	config;
	t_dongle_pool	pool;
	t_thread_context	*ctx;
	int					i;
	t_log_queue			logs;

	if (init_logs(&logs))
		return (1);
	config.number_of_coder = 4;
	config.number_of_compilation = 2;
	config.compilation_time = 100;
	config.debug_time = 200;
	config.refactor_time = 300;
	config.burnout_time = 1000;
	config.cooldown_time = 50;
	config.scheduler = FIFO;
	config.start_time = now();
	coders = malloc(sizeof(pthread_t) * config.number_of_coder);
	ctx = malloc(sizeof(t_thread_context));
	if (!ctx)
		return (1);
	if (!coders)
		return (1);
	if (init_dongle_pool(&pool, &config))
	{
		free(coders);
		return (1);
	}
	ctx->id = -1;
	ctx->config = &config;
	ctx->pool = &pool;
	ctx->logs = &logs;
	if (pthread_create(&tmp, NULL, monitor_routine, (void *) ctx))
		return (1);
	i = 0;
	while (i < config.number_of_coder)
	{
		ctx = malloc(sizeof(t_thread_context));
		if (!ctx)
			break;
		ctx->id = i + 1;
		ctx->config = &config;
		ctx->pool = &pool;
		ctx->logs = &logs;
		if (pthread_create(&coders[i], NULL, coder_routine, (void *) ctx))
		{
			free(pool.dongles);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_join(coders[i], NULL);
		i++;
	}
	pthread_join(tmp, NULL);
	free(pool.dongles);
	free(coders);
	return (0);
}
