/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:20:05 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 16:38:18 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
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
	logs->shutdown = 0;
	if (pthread_mutex_init(&logs->mutex, NULL))
		return (1);
	if (pthread_cond_init(&logs->cond, NULL))
		return (1);
	return (0);
}

int	init_queue(t_scheduler_queue *queue, t_config *config)
{
	queue->size = 0;
	if (config->scheduler == EDF)
		queue->sort = edf_sort;
	else
		queue->sort = fifo_sort;
	if (pthread_mutex_init(&queue->mutex, NULL))
		return (1);
	queue->entries = malloc(sizeof(t_queue_entry) * config->number_of_coder);
	if (!queue->entries)
		return (1);
	return (0);
}

void	clean_env(t_env *env)
{
	pthread_mutex_destroy(&env->pool.mutex);
	free(env->pool.dongles);
	pthread_mutex_destroy(&env->queue.mutex);
	free(env->queue.entries);
	pthread_mutex_destroy(&env->logs.mutex);
	pthread_cond_destroy(&env->logs.cond);
}

int	init_env(t_env *env, int argc, char **argv)
{
	if (validate_args(argc, argv, &env->config))
	{
		printf("Arguments are invalid\n");
		return (1);
	}
	if (init_logs(&env->logs))
		return (1);
	if (init_queue(&env->queue, &env->config))
		return (1);
	if (init_dongle_pool(&env->pool, &env->config))
	{
		free(env->queue.entries);
		return (1);
	}
	return (0);
}
