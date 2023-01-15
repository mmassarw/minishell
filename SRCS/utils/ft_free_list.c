/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:43:27 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 18:45:03 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_cmd(t_cmd *s_cmd)
{
	t_rdr	*current_rdr;
	t_rdr	*next_rdr;
	t_cmd	*next_cmd;

	while (s_cmd != NULL)
	{
		current_rdr = s_cmd->rdr;
		while (current_rdr != NULL)
		{
			current_rdr->file = (char *) ft_free(current_rdr->file);
			next_rdr = current_rdr->next;
			current_rdr = (t_rdr *) ft_free(current_rdr);
			current_rdr = next_rdr;
		}
		s_cmd->arg = (char **) ft_free_split(s_cmd->arg);
		next_cmd = s_cmd->next;
		s_cmd = (t_cmd *) ft_free(s_cmd);
		s_cmd = next_cmd;
	}
}

void	ft_free_env(t_env *env_list)
{
	t_env	*env_head;

	env_head = env_list;
	while (env_head)
	{
		env_list = env_head;
		env_head = env_list->next;
		env_list->key = (char *) ft_free(env_list->key);
		env_list->value = (char *) ft_free(env_list->value);
		env_list = (t_env *) ft_free(env_list);
	}
}
