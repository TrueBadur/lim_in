/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbartole <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 05:22:01 by mbartole          #+#    #+#             */
/*   Updated: 2019/07/13 11:27:24 by mbartole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

#define SIZE_OF_QUE 1000 * sizeof(t_node)
#define EDGE ((t_edge *)child->data)

/*
** deletes all not /was_rev/ edges from graph
** (walking it by BFS from finish)
*/

void	clean_graph(t_mngr *mngr, int iter)
{
	t_vector	*que;
	t_node		*cur;
	t_list		*child;
	t_list		*tmp;

	if (!(que = ft_vecinit(SIZE_OF_QUE)))
		ultimate_exit(mngr, MALLOC_ERROR);
	mngr->end->counter = iter;
	que = que_add(que, mngr->end, mngr);
	while (que->offset != que->len)
	{
		cur = que_popleft(que);
		child = cur->links;
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
		print_node(cur);
	}
	ft_vecdel((void **)&que);
}

char 	*print_one_lem(int num, char *name)
{
	char *s;

	s = ft_strjoin("L", ft_itoa(num));
	s = ft_strjoin(s, "-");
	s = ft_strjoin(s, name);
	s = ft_strjoin(s, " ");
	return (s);
}

/*
** moves ants towards finish by shortest paths first
*/

void 		move_lems(t_mngr *mngr, t_vector *output, t_node **ends)
{
	int i;
	int cur_lem;
	int count;
	t_list	*cur;
	t_edge	*edge;
	char 	*one;

	output++; // TODO trash
	count = 0;
	cur_lem = 1;
	printf("\n\n");
	while (count < mngr->ant_num)
	{
		i = 0;
		cur = mngr->end->links;
		while (cur)
		{
			if (ends[i])
			{
				edge = (t_edge *)cur->data;
				while (edge->to->counter == 0)
					edge = ((t_edge *)edge->to->links->data);
				if (edge->from == mngr->end)
					count++;
				while (edge->to != ends[i])
				{
					one = print_one_lem(edge->to->counter, edge->from->wrap->name);
					printf("%s", one); //TODO output
//					output = ft_vecpush(output, one, ft_strlen(one));
					edge->from->counter = edge->to->counter;
					edge = ((t_edge *)edge->to->links->data);
				}
				edge->to->counter--;
				edge->from->counter = cur_lem;
				cur_lem++;
				one = print_one_lem(edge->from->counter, edge->from->wrap->name);
				printf("%s", one); //TODO output
//				output = ft_vecpush(output, one, ft_strlen(one));
				if (edge->to->counter == 0)
					ends[i] = edge->from;
				if (ends[i] == mngr->end)
					ends[i] = NULL;
			}
			i++;
			cur = cur->next;
		}
		printf("\n\n");
	}
}

/*
** calculate total number of ants for every path,
** write it down to /mngr->start->counter/
*/

//TODO fast calculation after all weights are equal
void		calc_ants(t_vector *que, t_mngr *mngr, int n)
{
	t_pque		cur;

	while (n > 0)
	{
		cur = pop_que(que);
		((t_node *)cur.data)->counter++;
		n--;
		if (!(que = push_que(que, cur.data, cur.priority + 1)))
			ultimate_exit(mngr, MALLOC_ERROR);
	}
	ft_vecdel((void **)&que);
}

/*
** generate output string
*/

t_vector	*get_output(t_mngr *mngr, int size)
{
	t_vector	*output;
	t_vector	*que;
	int			ants[size];
	t_node		*ends[size];
	t_list		*cur;
	int 		i;
	t_edge		*ed;

	i = 0;
	ft_bzero(ends, sizeof(t_node*) * size);
	cur = mngr->end->links;
	if (!(que = ft_vecinit(SIZE_OF_QUE)))
		ultimate_exit(mngr, MALLOC_ERROR);
	while (cur)
	{
		ends[i] = ((t_edge*)cur->data)->to;
		ends[i]->counter = 0;
		ants[i] = 1;
		ed = ((t_edge *)ends[i]->links->data);
		while (1)
		{
			if (ed->to == mngr->start)
				break ;
			ants[i]++;
			ed = ((t_edge *)ends[i]->links->data);
			if (ed->to == mngr->start)
				break ;
			if (ed->from->wrap == ed->to->wrap)
			{
				ed->to = ((t_edge *)ed->to->links->data)->to;
			}
			print_node(ends[i]);  // TODO remove
			ends[i] = ed->to;
			ends[i]->counter = 0;
		}
		ends[i] = ed->from;
		ends[i]->counter = 0;
		if (!(que = push_que(que, ends[i], ants[i])))
			ultimate_exit(mngr, MALLOC_ERROR);
		printf("len = %i\n\n", ants[i]); // TODO remove
		cur = cur->next;
		i++;
	}
	calc_ants(que, mngr, mngr->ant_num);
	i = -1;          // TODO remove
	while (++i < size)  // TODO remove
		print_node(ends[i]); // TODO remove
	if (!(output = ft_vecinit(10 * mngr->ant_num * sizeof(char))))
		ultimate_exit(mngr, MALLOC_ERROR);
	move_lems(mngr, output, ends);
	return (output);
}

/*
** overall algorithm
*/

void		get_all_paths(t_mngr *mngr)
{
	int			i;
	t_vector	*output;

	set_weights(mngr);
	ft_printf("{Green}weights set{eof}\n\n"); // TODO remove
	if ((i = suurballe(mngr)) == -2)
		ultimate_exit(mngr, NO_PATHS_FOUND);
	ft_printf("{Blue}dijkstra has %i runs{eof}\n\n", -i - 2);  // TODO remove
	clean_graph(mngr, i - 1);
	ft_printf("{Green}graph cleaned{eof}\n\n"); // TODO remove
	output = get_output(mngr, ft_lstlen(mngr->end->links));
	ft_printf("%s", (char*)output->data);
	ft_vecdel((void **)&output);
}
