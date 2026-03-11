/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relaforg <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 11:34:16 by relaforg          #+#    #+#             */
/*   Updated: 2026/03/11 15:02:08 by relaforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fifo_sort(t_scheduler_queue *queue)
{
	int				i;
	t_queue_entry	tmp;

	i = 0;
	while (i < queue->size - 1)
	{
		if (queue->entries[i].request_time > queue->entries[i + 1].request_time)
		{
			tmp = queue->entries[i];
			queue->entries[i] = queue->entries[i + 1];
			queue->entries[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
}

void	edf_sort(t_scheduler_queue *queue)
{
	int				i;
	t_queue_entry	tmp;

	i = 0;
	while (i < queue->size - 1)
	{
		if (queue->entries[i].deadline > queue->entries[i + 1].deadline)
		{
			tmp = queue->entries[i];
			queue->entries[i] = queue->entries[i + 1];
			queue->entries[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
}
