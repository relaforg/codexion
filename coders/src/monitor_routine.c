/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:31:42 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 10:37:01 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>

int	check_coder_death(t_env *ctx)
{
	int	i;
	long long	t;

	t = now();
	i = 0;
	while (i < ctx->nb_coders_launched)
	{
		pthread_mutex_lock(&ctx->coders[i].mutex);
		if (t - ctx->config.start_time - ctx->coders[i].last_compile
			>= ctx->config.burnout_time && ctx->coders[i].nbr_compilation
			< ctx->config.number_of_compilation)
		{
			pthread_mutex_lock(&ctx->burnout.mutex);
			ctx->burnout.is_burnout = TRUE;
			pthread_mutex_unlock(&ctx->burnout.mutex);
			pthread_mutex_lock(&ctx->print.mutex);
			printf("%lld %d ", t - ctx->config.start_time, ctx->coders[i].id);
			printf("burned out\n");
			pthread_mutex_unlock(&ctx->print.mutex);
			pthread_mutex_unlock(&ctx->coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&ctx->coders[i].mutex);
		i++;
	}
	return (0);
}

int	check_end(t_env *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->nb_coders_launched)
	{
		pthread_mutex_lock(&ctx->coders[i].mutex);
		if (ctx->coders[i].nbr_compilation
			< ctx->config.number_of_compilation)
		{
			pthread_mutex_unlock(&ctx->coders[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&ctx->coders[i].mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *context)
{
	t_env				*ctx;

	ctx = (t_env *) context;

	while (1)
	{
		if (check_coder_death(ctx))
			break ;
		if (!check_end(ctx))
		{
			printf("\nAll coders are done !\n");
			break ;
		}
	}
	return (NULL);
}
