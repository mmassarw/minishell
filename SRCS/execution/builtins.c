/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 23:10:26 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/17 20:52:41 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_env(t_mini *mini)
{
	t_env	*env;

	env = mini->l_env;
	if (!env)
	{
		fd_printf(2, "env: permission denied\n");
		g_exit_code = ENV_FAIL_CODE;
		return ;
	}
	while (env)
	{
		if (env->initialised == true)
		{
			if (env->value == NULL)
				fd_printf (1, "%s=\n", env->key);
			else
				fd_printf(1, "%s=%s\n", env->key, env->value);
		}
		env = env->next;
	}
	g_exit_code = SUCCESS;
}

// prints the present working directory
// print_pwd does not take any arguments and in case
// of failure it sets the exit code to PWD_FAIL_CODE
void	print_pwd(void)
{
	char	cwd[2056];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		fd_printf(1, "%s\n", cwd);
		g_exit_code = 0;
	}
	else
	{
		fd_printf (2, "this error is coming from print_pwd function\n");
		g_exit_code = PWD_FAIL_CODE;
	}
}
