/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 14:58:28 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/13 01:02:49 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void) argc;
	(void) argv;
	env_list = ft_parse_env((const char **)envp);
	ft_free_env(env_list);
}
