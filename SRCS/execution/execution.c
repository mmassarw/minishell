/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 01:05:21 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/13 02:09:13 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_pwd(t_mini *mini)
{
	if (getcwd(mini->cwd, sizeof(mini->cwd)) != NULL)
		printf("%s\n", mini->cwd);
	else
		perror("bash");
}

void	safe_exit(t_mini *mini, int flag)
{
	free (mini->read_line);
	exit (flag);
}

void	parse_input(t_mini *mini)
{
	if (strncmp(mini->read_line, "pwd", 4) == 0)
		print_pwd(mini);
	else if (strncmp(mini->read_line, "exit", 5) == 0)
		safe_exit(mini, 0);
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

int	main(int ac, char **av, char **env)
{
	t_mini	mini;

	(void) ac;
	(void) env;
	(void) av;
	take_input(&mini);
}
