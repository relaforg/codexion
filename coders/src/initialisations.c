/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:20:05 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 16:50:41 by relaforg         ###   ########.fr       */
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

int	init_burnout(t_burnout *burnout)
{
	burnout->is_burnout = FALSE;
	if (pthread_mutex_init(&burnout->mutex, NULL))
		return (1);
	return (0);
}

int	init_coders(t_coder **coders, t_config *config)
{
	int	i;

	*coders = malloc(sizeof(t_coder) * config->number_of_coder);
	if (!*coders)
		return (1);
	i = 0;
	while (i < config->number_of_coder)
	{
		(*coders)[i].id = i + 1;
		(*coders)[i].last_compile = 0;
		(*coders)[i].nbr_compilation = 0;
		if (pthread_mutex_init(&(*coders)[i].mutex, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(*coders)[i].mutex);
			free(*coders);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_queue(t_scheduler_queue *queue, t_config *config, t_coder *coders)
{
	int	i;

	queue->size = 0;
	if (config->scheduler == EDF)
		queue->sort = edf_sort;
	else
		queue->sort = fifo_sort;
	if (pthread_mutex_init(&queue->mutex, NULL))
		return (1);
	queue->entries = malloc(sizeof(t_queue_entry) * config->number_of_coder);
	if (!queue->entries)
	{
		pthread_mutex_destroy(&queue->mutex);
		return (1);
	}
	i = 0;
	while (i < config->number_of_coder)
	{
		queue->entries[queue->size].coder = &coders[i];
		queue->entries[queue->size].request_time = now();
		queue->entries[queue->size].deadline = config->burnout_time;
		queue->size++;
		i++;
	}
	return (0);
}

int	init_env(t_env *env, int argc, char **argv)
{
	if (validate_args(argc, argv, &env->config))
	{
		printf("Arguments are invalid\n");
		return (1);
	}
	if (pthread_mutex_init(&env->print.mutex, NULL))
		return (1);
	if (init_burnout(&env->burnout))
		return (1);
	if (init_coders(&env->coders, &env->config))
		return (1);
	if (init_queue(&env->queue, &env->config, env->coders))
	{
		free(env->coders);
		return (1);
	}
	if (init_dongle_pool(&env->pool, &env->config))
	{
		free(env->queue.entries);
		free(env->coders);
		return (1);
	}
	return (0);
}
