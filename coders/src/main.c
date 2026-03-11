/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:38:50 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/11 12:02:25 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>

int	launch_monitor(t_env *env)
{
	t_thread_context	*ctx;

	ctx = malloc(sizeof(t_thread_context));
	if (!ctx)
		return (1);
	ctx->id = -1;
	ctx->config = &env->config;
	ctx->pool = &env->pool;
	ctx->logs = &env->logs;
	ctx->queue = &env->queue;
	if (pthread_create(&env->monitor, NULL, monitor_routine, (void *)ctx))
	{
		free(ctx);
		return (1);
	}
	return (0);
}

int	launch_coders(pthread_t *coders, t_env *env)
{
	t_thread_context	*ctx;

	env->nb_coders_launched = 0;
	while (env->nb_coders_launched < env->config.number_of_coder)
	{
		ctx = malloc(sizeof(t_thread_context));
		if (!ctx)
		{
			send_log(-1, SHUTDOWN, &env->logs);
			return (1);
		}
		ctx->id = env->nb_coders_launched + 1;
		fill_context(ctx, env);
		if (pthread_create(&coders[env->nb_coders_launched], NULL,
				coder_routine, (void *)ctx))
		{
			free(ctx);
			send_log(-1, SHUTDOWN, &env->logs);
			return (1);
		}
		env->nb_coders_launched++;
	}
	return (0);
}

void	join_threads(t_env *env, pthread_t *coders)
{
	int	i;

	i = 0;
	while (i < env->nb_coders_launched)
	{
		pthread_join(coders[i], NULL);
		i++;
	}
	pthread_join(env->monitor, NULL);
}

int	run(t_env *env)
{
	pthread_t	*coders;

	coders = malloc(sizeof(pthread_t) * env->config.number_of_coder);
	if (!coders)
		return (1);
	if (launch_monitor(env))
		return (free(coders), 1);
	if (launch_coders(coders, env))
	{
		join_threads(env, coders);
		free(coders);
		return (1);
	}
	join_threads(env, coders);
	free(coders);
	return (0);
}

int	main(int argc, char **argv)
{
	t_env	env;

	if (init_env(&env, argc, argv))
		return (1);
	if (run(&env))
	{
		clean_env(&env);
		return (1);
	}
	clean_env(&env);
	return (0);
}
