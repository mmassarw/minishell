/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/30 00:46:22 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	parse_input(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->l_cmd;
	handle_heredoc(mini);
	while (cmd)
	{
		if (ft_redirect(mini, cmd) != 0)
		{
			cmd = cmd->next;
			continue ;
		}
		if (cmd->arg[0] && builtin_check(mini, cmd) == 0)
		{
			close_rdr_back(cmd);
			cmd = cmd->next;
			continue ;
		}
		else if (cmd->arg[0] && access(cmd->arg[0], X_OK) == 0)
			execute_in_dir(mini, cmd);
		else if (cmd->arg[0])
			execute_pathed_cmd(mini, cmd);
		close_rdr_back(cmd);
		cmd = cmd->next;
	}
}
