/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:16:48 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/11 11:17:03 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ensure_in_queue(t_thread_context *ctx, long long last_compile)
{
	int		i;

	i = 0;
	while (i < ctx->queue->size)
	{
		if (ctx->queue->entries[i].coder_id == ctx->id)
			break ;
		i++;
	}
	if (i == ctx->queue->size)
	{
		ctx->queue->entries[ctx->queue->size].coder_id = ctx->id;
		ctx->queue->entries[ctx->queue->size].request_time = now();
		ctx->queue->entries[ctx->queue->size].deadline = last_compile
			+ ctx->config->burnout_time;
		ctx->queue->size++;
	}
}
