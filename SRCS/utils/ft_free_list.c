/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:43:27 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/16 02:08:29 by mmassarw         ###   ########.fr       */
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
		if (s_cmd->arg)
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

void	ft_free_cycle(t_mini *mini)
{
	ft_free_cmd(mini->l_cmd);
	mini->rl = (char *) ft_free(mini->rl);
	mini->token = (char **) ft_free_split(mini->token);
}

void	ft_free_all(t_mini *mini)
{
	ft_free_cycle(mini);
	ft_free_env(mini->l_env);
}

void	ft_exit_shell(t_mini *mini, int error)
{
	g_exit_code = error;
	ft_free_all(mini);
	exit(g_exit_code);
}
