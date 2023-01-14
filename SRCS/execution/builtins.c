/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 23:10:26 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/14 06:35:15 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_env(t_mini *mini)
{
	t_env	*env;

	env = mini->l_env;
	if (!env)
	{
		printf("this error is comming from function print env\n");
		exit(69);
	}
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

void	print_pwd(t_mini *mini)
{
	if (getcwd(mini->cwd, sizeof(mini->cwd)) != NULL)
		printf("%s\n", mini->cwd);
	else
	{
		printf("this error is coming from print_pwd function\n");
		perror("bash");
	}
}
