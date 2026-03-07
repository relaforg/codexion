/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:38:50 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/07 13:53:46 by relaforg         ###   ########.fr       */
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

int	init_message(t_message *message)
{
	message->coder_id = -1;
	message->type = NONE;
	if (pthread_mutex_init(&message->mutex, NULL))
		return (1);
	if (pthread_cond_init(&message->cond, NULL))
		return (1);
	return (0);
}

int	main(void)
{
	pthread_t	tid;
	t_config	config;
	t_dongle_pool	pool;
	t_thread_context	ctx;
	int					i;
	t_message			message;

	// if (pthread_create(&tid, NULL, monitor_routine, NULL))
	// 	return (1);
	config.number_of_coder = 4;
	config.number_of_compilation = 2;
	config.compilation_time = 100;
	config.debug_time = 200;
	config.refactor_time = 300;
	config.burnout_time = 1000;
	config.cooldown_time = 50;
	config.scheduler = FIFO;
	if (init_message(&message))
		return (1);
	if (init_dongle_pool(&pool, &config))
		return (1);
	ctx.config = &config;
	ctx.pool = &pool;
	ctx.message = &message;
	i = 0;
	while (i < config.number_of_coder)
	{
		ctx.id = i + 1;
		if (pthread_create(&tid, NULL, coder_routine, (void *) &ctx))
		{
			free(pool.dongles);
			return (1);
		}
		pthread_join(tid, NULL);
		i++;
	}
	free(pool.dongles);
	return (0);
}
