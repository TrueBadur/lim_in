/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 19:01:09 by mbartole          #+#    #+#             */
/*   Updated: 2019/07/23 21:55:22 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

#define EDGE ((t_edge *)child->data)

/*
** set array /ends/ and get length of the longest path
*/

static int	longest_path(t_node *start, t_node *end, t_node **ends)
{
	t_list	*child;
	int		i;
	int		max;
	int		len;

	max = 0;
	child = end->links;
	i = -1;
	while (child)
	{
		len = get_path_len(EDGE->to, start, 1, &ends[++i]);
		max = len > max ? len : max;
		child = child->next;
	}
	return (max);
}

/*
** calculate total number of ants for every path,
** write it down to /node->counter/ for start of path
** (in fact it's /mngr->start->child/s)
*/

void		calc_ants(t_mngr *mngr, int size, t_node **ends)
{
	int		max;
	int		sum;
	int		i;

	max = longest_path(mngr->start, mngr->end, ends);
	sum = mngr->ant_num;
	i = -1;
	while (++i < size)
	{
		ends[i]->counter = max - ends[i]->counter;
		sum -= ends[i]->counter;
//		print_node(ends[i]); // TODO print
	}
//	ft_printf("sum %i\n", sum); // TODO print
	max = sum % size; // ostatok
	sum = sum / size; // po skolko v kazdoe
//	ft_printf("v kazdoe %i, ostatok %i\n", sum, max); // TODO print
	i = -1;
	while (++i < size)
	{
		ends[i]->counter += sum;
		if (max-- > 0)
			ends[i]->counter += 1;
	}
}

int			move_one_ant(t_edge *edge, t_vector **output, int num, char *name)
{
	char 	*s;

	edge->from->counter = num;
	*output = ft_vecpush(*output, "L", 1);
	s = ft_itoa(num);
	*output = ft_vecpush(*output, s, ft_strlen(s));
	free(s);
	*output = ft_vecpush(*output, "-", 1);
	*output = ft_vecpush(*output, name, ft_strlen(name));
	*output = ft_vecpush(*output, " ", 1);
	return (1);
}

int 		get_one_line(int *finishs, t_node **ends, t_vector **output, t_mngr *mngr, int *cur_lem)
{
	int 	count;
	int 	i;
	t_list	*cur;
	t_edge	*edge;

	i = -1;
	cur = mngr->end->links;
	count = 0;
	while (cur && ++i > -1)
	{
		if (finishs[i])
			move_one_ant((t_edge *)cur->data, output, finishs[i], mngr->end->wrap->name);
		if (ends[i] && ++count)
		{
			edge = (t_edge *)cur->data;
			if (edge->to->counter != 0)
				finishs[i] = edge->to->counter;
			else
				while (edge->to->counter == 0)  // just pass
					edge = ((t_edge *)edge->to->links->data);
			while (edge->to != ends[i] && move_one_ant(edge, output, edge->to->counter, edge->to->wrap->name))
				edge = (t_edge *)edge->to->links->data;
			if (((t_edge*)ends[i]->links->data)->to == mngr->start)
			{
				move_one_ant(edge, output, (*cur_lem)++, edge->to->wrap->name);
				if (--ends[i]->counter == 0)
					ends[i] = edge->from;
			}
			else
			{
				move_one_ant(edge, output, edge->to->counter, edge->to->wrap->name);
				ends[i] = edge->from == mngr->end ? NULL : edge->from;
			}
		}
		cur = cur->next;
	}
	return (count);
}

/*
** moves ants towards finish by shortest paths first
*/

t_vector	*move_ants(t_mngr *mngr, t_vector *output, int size)
{
	int		cur_lem;
	int		count;
	int		finishs[size];
	t_node	*ends[size];

	calc_ants(mngr, ft_lstlen(mngr->end->links), ends);
	ft_bzero(finishs, sizeof(int) * size);
	cur_lem = 1;
	count = 1;
	while (count)
	{
		count = get_one_line(finishs, ends, &output, mngr, &cur_lem);
		output = ft_vecpush(output, "\n", 1);
	}
	return (output);
}
