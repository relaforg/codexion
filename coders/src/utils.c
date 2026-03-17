/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:01:37 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 09:33:35 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fill_context(t_thread_context *ctx, t_env *env)
{
	ctx->config = &env->config;
	ctx->pool = &env->pool;
	ctx->print = &env->print;
	ctx->burnout = &env->burnout;
	ctx->queue = &env->queue;
}
