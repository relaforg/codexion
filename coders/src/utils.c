/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:01:37 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/11 12:03:04 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fill_context(t_thread_context *ctx, t_env *env)
{
	ctx->config = &env->config;
	ctx->pool = &env->pool;
	ctx->logs = &env->logs;
	ctx->queue = &env->queue;
}
