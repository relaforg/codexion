/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:34:27 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/09 09:37:51 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h" 
#include <pthread.h>
#include <stdio.h>

void	*coder_routine(void *ctx)
{
	int				id;
	t_config		*config;
	t_dongle_pool	*pool;
	t_log_queue		*logs;

	id = ((t_thread_context *) ctx)->id;
	config = ((t_thread_context *) ctx)->config;
	pool = ((t_thread_context *) ctx)->pool;
	logs = ((t_thread_context *) ctx)->logs;
	printf("id: %d\n", id);
	printf("config: %p\n", config);
	printf("pool: %p\n", pool);
	printf("logs: %p\n", logs);
	return (0);
}

void	*monitor_routine(void *ctx)
{
	t_log_queue		*logs;

	logs = (t_log_queue*) ctx;
	printf("Monitor:\n");
	printf("logs: %p\n", logs);
	// while (pthread_cond_wait() != 0)
	// {
	// 	continue;
	// }
	return (0);
}
