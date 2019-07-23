/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_all_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 05:22:01 by mbartole          #+#    #+#             */
/*   Updated: 2019/07/23 21:11:46 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

#define SIZE_OF_QUE 1000 * sizeof(t_node)
#define EDGE ((t_edge *)child->data)

/*
** deletes all not /was_rev/ edges from graph
** (walking it by BFS from finish)
*/

static void	clean_graph(t_mngr *mngr, int iter)
{
	t_vector	*que;
	t_list		*child;
	t_list		*tmp;

	if (!(que = ft_vecinit(SIZE_OF_QUE)))
		ultimate_exit(mngr, MALLOC_ERROR);
	mngr->end->counter = iter;
	que = que_add(que, mngr->end, mngr);
	while (que->offset != que->len && (child = que_popleft(que)->links))
		while (child)
		{
			if (EDGE->to->counter != iter)
			{
				EDGE->to->counter = iter;
				que = que_add(que, EDGE->to, mngr);
			}
			if (!EDGE->was_rev)
			{
				tmp = pop_edge(&(EDGE->from->links), EDGE);
				free(tmp->data);
				free(tmp);
			}
			child = child->next;
		}
	ft_vecdel((void **)&que);
}

/*
** overall algorithm
*/

void		do_all_job(t_mngr *mngr)
{
	int			i;
	int			size;
	t_vector	*output;
	t_list		*ends;

	set_weights(mngr);
//	ft_printf("{Green}weights set{eof}\n\n"); // TODO print
	ends = NULL;
	if ((i = suurballe(mngr, &ends)) == -2)
		ultimate_exit(mngr, NO_PATHS_FOUND);
	ft_lstdel(&ends, NULL);
//	ft_printf("{Blue}dijkstra has %i runs{eof}\n\n", -i - 2);  // TODO print
	clean_graph(mngr, i - 1);
	size = ft_lstlen(mngr->end->links);
//	ft_printf("{Green}graph cleaned{eof}\n\n"); // TODO print
//	calc_ants(mngr->end, mngr->start, mngr->ant_num, size);
	if (!(output = ft_vecinit(1000 * mngr->ant_num * sizeof(char))))
		ultimate_exit(mngr, MALLOC_ERROR);
//	ft_printf("#num of paths %i\n", size);
//	calc_ants(mngr->end, mngr->start, mngr->ant_num, size);
	output = move_ants(mngr, output, size);
	ft_printf("\n%s", (char*)output->data);
	ft_vecdel((void **)&output);
}