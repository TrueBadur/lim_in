/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_room.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehugh-be <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 19:11:10 by ehugh-be          #+#    #+#             */
/*   Updated: 2019/07/09 19:11:28 by ehugh-be         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lem_in.h"

char	*get_node_name(char *line)
{
	char	*ret;
	int		i;

	i = ft_strchr(line, ' ') - line;
	if (!(ret = malloc(i + 1)))
		return (NULL);
	ft_strncpy(ret, line, i);
	ret[i] = '\0';
	return (ret);
}

t_elt	get_room(t_mngr *mngr, char *line)
{
	t_node	*node;

	if ((node = malloc(sizeof(t_node))) == NULL)
		ultimate_exit(&mngr);
	if (!(node->name = get_node_name(line)))
		ultimate_exit(&mngr);
	node->links = NULL;
	node->wgth = -1;
	if (ft_avlsearch(mngr->all_rooms, node->name, 0)) //TODO adapt avl to string keys and compare functions
	{
		free(node->name);
		free(node);
		return (ERROR);
	}
	ft_avlins(mngr->all_rooms, ft_avlnew(node, node->name, sizeof(t_node)));
	return (ROOM);
}