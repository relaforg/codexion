/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:38:50 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/07 09:51:04 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(void)
{
	t_config	config;

	config.number_of_coder = 4;
	config.number_of_compilation = 2;
	config.compilation_time = 100;
	config.debug_time = 200;
	config.refactor_time = 300;
	config.burnout_time = 1000;
	config.cooldown_time = 50;
	config.scheduler = FIFO;
	return (0);
}
