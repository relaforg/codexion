/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 15:49:16 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 15:01:19 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	compile(t_thread_context *ctx)
{
	send_log(ctx->id, COMPILE, ctx->logs);
	usleep(ctx->config->compilation_time * 1000);
}

void	debug(t_thread_context *ctx)
{
	send_log(ctx->id, DEBUG, ctx->logs);
	usleep(ctx->config->debug_time * 1000);
}

void	refactor(t_thread_context *ctx)
{
	send_log(ctx->id, REFACTOR, ctx->logs);
	usleep(ctx->config->refactor_time * 1000);
}
