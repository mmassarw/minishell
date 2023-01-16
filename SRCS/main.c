/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 16:49:59 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/16 17:30:25 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	take_input(t_mini *mini)
{
	while (1)
	{
		mini->rl = readline (BLUE_FONT"mminishell-3.2> " RESET_FONT);
		if (mini->rl == NULL)
			ft_exit_shell(mini, 0);
		if (mini->rl[0] != '\0')
			add_history(mini->rl);
		mini->token = ft_split(mini->rl, ' ');
		if (!mini->token)
			ft_exit_shell(mini, 137);
		ft_parse_token(mini, mini->token);
		// ft_print_cmd(mini->l_cmd);
		parse_input(mini);
		ft_free_cycle(mini);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void) argc;
	(void) argv;
	g_exit_code = 0;
	ft_bzero(&mini, sizeof(t_mini));
	ft_parse_env(&mini, (const char **)envp);
	take_input(&mini);
}
