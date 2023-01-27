/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:49:59 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/27 22:49:58 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	take_input(t_mini *mini)
{
	while (1)
	{
		mini->rl = read_line_colored(random_between(1, 6));
		if (mini->rl == NULL)
			ft_exit_shell(mini, 0, "exit", 1);
		if (mini->rl[0] != '\0')
			add_history(mini->rl);
		ft_tokenize(mini);
		if (mini->token)
			ft_parse_token(mini, mini->token);
		// ft_print_cmd(mini->l_cmd);
		if (mini->l_cmd)
			parse_input(mini);
		ft_free_cycle(mini);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void) argc;
	(void) argv;
	ft_bzero(&mini, sizeof(t_mini));
	ft_parse_env(&mini, (const char **)envp);
	take_input(&mini);
}
