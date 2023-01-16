/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:57:41 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/16 17:45:34 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * @brief prints an array of strings
 * 
 * @param split 
 */
void	ft_print_split(char **split)
{
	while (*split)
		printf("%s ", *split++);
}

/**
 * @brief prints the cmd list <s_head>
 * 
 * @param s_head 
 */
void	ft_print_cmd(t_cmd *s_head)
{
	int		list;
	t_cmd	*current;
	t_rdr	*rdr;

	list = 0;
	current = s_head;
	while (current != NULL)
	{
		printf(">>>>>>>> %d <<<<<<<<<\n", list++);
		printf("Command: ");
		ft_print_split(current->arg);
		printf("\nRedirections: ");
		rdr = current->rdr;
		while (rdr != NULL)
		{
			printf("%i %s ", rdr->e_rdr, rdr->file);
			rdr = rdr->next;
		}
		printf("\n\n");
		current = current->next;
	}
}
