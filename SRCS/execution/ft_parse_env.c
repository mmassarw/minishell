/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 18:27:45 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 01:34:56 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * @brief Parses through the enviroment <envp> and allocates the keys
 * and their values into a singly linked list.
 * 
 * @param envp 
 * @return The enviromental linked list
 */

void	increase_shlvl(t_env *env)
{
	t_env	*tmp;
	int		lvl;

	lvl = 0;
	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, "SHLVL", 5) == 0)
		{
			lvl = ft_atoi(tmp->value);
			lvl++;
			free (tmp->value);
			if (lvl > 999)
				tmp->value = NULL;
			else
				tmp->value = ft_itoa(lvl);
		}
		tmp = tmp->next;
	}
}

t_env	*ft_parse_env(const char **envp)
{
	t_env	*env_new;
	t_env	*env_head;
	t_env	*env_tail;

	env_head = NULL;
	env_tail = NULL;
	while (*envp)
	{
		env_new = (t_env *) ft_calloc(1, sizeof(t_env));
		// if (!env_new)
		// 	error
		env_new->key = ft_substr(*envp, 0, (ft_strchr(*envp, '=') - *envp));
		env_new->value = ft_strdup(ft_strchr(*envp++, '=') + 1);
		env_new->initialised = true;
		// if (!env_new->key || !env_new->value)
		// 	error
		env_new->next = NULL;
		if (env_head == NULL)
			env_head = env_new;
		else
			env_tail->next = env_new;
		env_tail = env_new;
	}
	increase_shlvl(env_head);
	return (env_head);
}

void	ft_free_env(t_env *env_list)
{
	t_env	*env_head;

	env_head = env_list;
	while (env_head)
	{
		env_list = env_head;
		env_head = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
	}
}
