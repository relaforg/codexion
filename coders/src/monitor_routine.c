/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:31:42 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/13 09:30:56 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>

t_log_entry	dequeue_log(t_log_queue *logs)
{
	t_log_entry	msg;

	pthread_mutex_lock(&logs->mutex);
	while (logs->count == 0)
		pthread_cond_wait(&logs->cond, &logs->mutex);
	msg = logs->entries[logs->head];
	logs->head = (logs->head + 1) % 1024;
	logs->count--;
	pthread_mutex_unlock(&logs->mutex);
	return (msg);
}

int	handle_log_entry(t_coder *coders, t_log_entry msg, long long start_time)
{
	printf("%lld %d ", msg.timestamp - start_time, msg.coder_id);
	if (msg.message == DONGLE)
	{
		printf("has taken a dongle\n");
		printf("%lld %d ", msg.timestamp - start_time, msg.coder_id);
		printf("has taken a dongle\n");
	}
	else if (msg.message == COMPILE)
	{
		coders[msg.coder_id - 1].last_compile = now() - start_time;
		printf("is compiling\n");
	}
	else if (msg.message == DEBUG)
		printf("is debugging\n");
	else if (msg.message == REFACTOR)
		printf("is refactoring\n");
	else if (msg.message == BURNOUT)
	{
		printf("burned out\n");
		return (0);
	}
	return (1);
}

void	check_coder_death(t_env *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->nb_coders_launched)
	{
		if (now() - ctx->config.start_time - ctx->coders[i].last_compile
			>= ctx->config.burnout_time)
			send_log(i + 1, BURNOUT, &ctx->logs);
		i++;
	}
}

void	*monitor_routine(void *context)
{
	t_env				*ctx;
	int					count;
	t_log_entry			msg;

	ctx = (t_env *) context;
	count = 0;
	while (count < ctx->nb_coders_launched)
	{
		check_coder_death(ctx);
		msg = dequeue_log(&ctx->logs);
		if (msg.message == DONE)
		{
			count++;
			continue ;
		}
		else if (msg.message == SHUTDOWN)
			return (NULL);
		if (!handle_log_entry(ctx->coders, msg, ctx->config.start_time))
			return (NULL);
	}
	printf("\nAll coders are done !\n");
	return (NULL);
}
