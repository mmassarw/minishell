/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 01:05:21 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/14 06:36:18 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	safe_exit(t_mini *mini, int flag)
{
	free (mini->read_line);
	exit (flag);
}

void	parse_input(t_mini *mini)
{
	char	**args;

	if (ft_strncmp(mini->read_line, "pwd", 4) == 0)
		print_pwd(mini);
	else if (ft_strncmp(mini->read_line, "exit", 5) == 0
		|| mini->read_line[0] == 'q')
		safe_exit(mini, 0);
	else if (ft_strncmp(mini->read_line, "env", 4) == 0)
		print_env(mini);
	else if (ft_strncmp(mini->read_line, "echo", 4) == 0)
	{
		args = ft_split(mini->read_line, ' ');
		ft_echo(&args[1]);
		ft_free_split(args);
	}
	else if (!mini->read_line[0])
		return ;
	else
		printf(RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
			mini->read_line);
}

void	take_input(t_mini *mini)
{
	mini->read_line = NULL;
	while (1)
	{
		mini->read_line = readline ("mminishell-3.2$ " RESET_FONT);
		if (mini->read_line == NULL)
			exit(0);
		parse_input(mini);
		free(mini->read_line);
	}
}

int	main(int ac, char **av, const char **env)
{
	t_mini	mini;
	t_env	*l_env;

	(void) ac;
	(void) av;
	l_env = NULL;
	mini.env = (char *)env;
	l_env = ft_parse_env(env);
	mini.l_env = l_env;
	take_input(&mini);
	ft_free_env(mini.l_env);
}
