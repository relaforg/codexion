/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 13:34:08 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/10 16:37:29 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>
#include <stdlib.h>

int	parse_args(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (1);
	config->number_of_coder = atoi(argv[1]);
	config->burnout_time = atoi(argv[2]);
	config->compilation_time = atoi(argv[3]);
	config->debug_time = atoi(argv[4]);
	config->refactor_time = atoi(argv[5]);
	config->number_of_compilation = atoi(argv[6]);
	config->cooldown_time = atoi(argv[7]);
	if (!strcmp(argv[8], "fifo"))
		config->scheduler = FIFO;
	else if (!strcmp(argv[8], "edf"))
		config->scheduler = EDF;
	else
		config->scheduler = NONE;
	return (0);
}

int	validate_args(int argc, char **argv, t_config *config)
{
	if (parse_args(argc, argv, config))
		return (1);
	if (config->number_of_coder <= 0
		|| config->burnout_time <= 0
		|| config->compilation_time <= 0
		|| config->debug_time <= 0
		|| config->refactor_time <= 0
		|| config->number_of_compilation <= 0
		|| config->cooldown_time <= 0
		|| config->scheduler == NONE)
		return (1);
	config->start_time = now();
	return (0);
}
