/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 18:27:45 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/17 22:04:43 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * @brief incriments shell lvl <SHLVL>
 * 
 * @param env 
 * @param mini 
 */

void	add_shlvl(t_env *env)
{
	t_env	*temp;
	t_env	*new;

	temp = env;
	new = ft_calloc(sizeof(t_env), 1);
	new->initialised = true;
	new->key = ft_strdup("SHLVL");
	new->value = ft_strdup("1");
	new->next = NULL;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	fd_printf(1, "SHLVL 1 added\n");
}

void	increase_shlvl(t_env *env)
{
	t_env	*tmp;
	int		lvl;
	bool	found;

	found = false;
	lvl = 0;
	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->key, "SHLVL", 6) == 0)
		{
			found = true;
			lvl = ft_atoi(tmp->value);
			lvl++;
			tmp->value = (char *) ft_free(tmp->value);
			if (lvl > 999 || lvl < 0)
				tmp->value = NULL;
			else
				tmp->value = ft_itoa(lvl);
		}
		tmp = tmp->next;
	}
	if (!found)
		add_shlvl(env);
	else
		fd_printf(1, "SHLVL increased to %d\n", lvl);
}

/**
 * @brief Parses through the enviroment <envp> and allocates the keys
 * and their values into a singly linked list.
 * 
 * @param envp 
 * @return The enviromental linked list
 */
void	ft_parse_env(t_mini *mini, const char **envp)
{
	t_env	*env_new;
	t_env	*env_tail;

	if (!envp || !envp[0])
	{
		fd_printf(2, "no env, handle error when there is no env it creates a basic env\n");
		exit (1);
	}
	env_tail = NULL;
	while (*envp)
	{
		env_new = (t_env *) ft_calloc(1, sizeof(t_env));
		if (!env_new)
			ft_exit_shell(mini, 137, "malloc fail\n", 2);
		env_new->key = ft_substr(*envp, 0, (ft_strchr(*envp, '=') - *envp));
		env_new->value = ft_strdup(ft_strchr(*envp++, '=') + 1);
		env_new->initialised = true;
		if (!env_new->key || !env_new->value)
			ft_exit_shell(mini, 137, "malloc fail\n", 2);
		env_new->next = NULL;
		if (mini->l_env == NULL)
			mini->l_env = env_new;
		else
			env_tail->next = env_new;
		env_tail = env_new;
	}
	increase_shlvl(mini->l_env);
}
