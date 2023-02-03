/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mani_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 12:26:50 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/03 14:02:18 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	pop_node(t_token **list, t_token *node)
{
	if (*list == NULL || node == NULL)
		return ;
	if (node->prev != NULL)
		node->prev->next = node->next;
	if (node->next != NULL)
		node->next->prev = node->prev;
	if (*list == node)
		*list = node->next;
	node->prev = NULL;
	node->next = NULL;
	ft_free_ltoken(node);
}

void	add_node_middle(t_token *node_before, t_token *new_node)
{
	if (node_before->next)
	{
		node_before->next->prev = new_node;
		new_node->next = node_before->next;
	}
	node_before->next = new_node;
	new_node->prev = node_before;
}