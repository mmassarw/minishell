/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 01:05:21 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/17 02:49:40mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// parse_input expects clean input from l_cmd->arg[0]
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function
void	parse_input(t_mini *mini)
{
	if (ft_strncmp(mini->l_cmd->arg[0], "pwd", 4) == 0)
		print_pwd();
	else if (ft_strncmp(mini->l_cmd->arg[0], "exit", 5) == 0
		|| mini->l_cmd->arg[0][0] == 'q')
	{
		ft_exit(mini->l_cmd->arg, mini);
	}
	else if (ft_strncmp(mini->l_cmd->arg[0], "env", 4) == 0)
		print_env(mini);
	else if (ft_strncmp(mini->l_cmd->arg[0], "echo", 5) == 0)
	{
		ft_echo(&mini->l_cmd->arg[1]);
	}
	else if (ft_strncmp(mini->l_cmd->arg[0], "export", 7) == 0)
	{
		ft_export(&mini->l_cmd->arg[1], mini);
	}
	else if (!mini->l_cmd->arg[0][0])
		return ;
	else
	{
		g_exit_code = COMMAND_FAIL;
		printf(RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
			mini->l_cmd->arg[0]);
	}
}
