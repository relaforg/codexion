/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:31:42 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 15:32:19 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

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

int	handle_log_entry(t_log_entry msg, long long start_time)
{
	printf("%lld %d ", msg.timestamp - start_time, msg.coder_id);
	if (msg.message == DONGLE)
		printf("has taken a dongle\n");
	else if (msg.message == COMPILE)
		printf("is compiling\n");
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

void	*monitor_routine(void *context)
{
	t_thread_context	*ctx;
	int					count;
	t_log_entry			msg;

	ctx = (t_thread_context *) context;
	count = 0;
	while (count < ctx->config->number_of_coder)
	{
		msg = dequeue_log(ctx->logs);
		if (msg.message == DONE)
		{
			count++;
			continue ;
		}
		if (!handle_log_entry(msg, ctx->config->start_time))
			return (free(context), NULL);
	}
	printf("\nAll coders are done !\n");
	free(context);
	return (NULL);
}
