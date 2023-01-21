/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:57:41 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/21 02:35:01 by hakaddou         ###   ########.fr       */
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
		printf("\n>>>>>>>> cmd %d <<<<<<<<<\n", ++list);
		printf("Command: %s, args: ", current->arg[0]);
		ft_print_split(current->arg + 1);
		printf("\nRedirections: ");
		rdr = current->rdr;
		while (rdr != NULL)
		{
			printf("%i %s, ", rdr->e_rdr, rdr->file);
			rdr = rdr->next;
		}
		printf("\n");
		current = current->next;
	}
}
