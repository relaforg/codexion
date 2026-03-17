/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 16:48:40 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/17 16:49:08 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	clean_env(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->config.number_of_coder)
	{
		pthread_mutex_destroy(&env->coders[i].mutex);
		i++;
	}
	free(env->coders);
	pthread_mutex_destroy(&env->pool.mutex);
	free(env->pool.dongles);
	pthread_mutex_destroy(&env->queue.mutex);
	free(env->queue.entries);
	pthread_mutex_destroy(&env->print.mutex);
}
