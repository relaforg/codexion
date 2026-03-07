/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 10:34:27 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/07 13:49:22 by relaforg         ###   ########.fr       */
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

	id = ((t_thread_context *) ctx)->id;
	config = ((t_thread_context *) ctx)->config;
	pool = ((t_thread_context *) ctx)->pool;
	printf("%d\n", id);
	printf("config: %p\n", config);
	printf("pool: %p\n", pool);
	return (0);
}

void	*monitor_routine(void *ctx)
{
	(void) ctx;
	// while (pthread_cond_wait() != 0)
	// {
	// 	continue;
	// }
	return (0);
}
