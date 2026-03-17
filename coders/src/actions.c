/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 15:49:16 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 09:27:26 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	compile(t_thread_context *ctx)
{
	print(ctx, COMPILE);
	usleep(ctx->config->compilation_time * 1000);
}

void	debug(t_thread_context *ctx)
{
	print(ctx, DEBUG);
	usleep(ctx->config->debug_time * 1000);
}

void	refactor(t_thread_context *ctx)
{
	print(ctx, REFACTOR);
	usleep(ctx->config->refactor_time * 1000);
}
