/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/01 20:39:22 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec_children_cmds(t_mini *mini, t_cmd *cmd)
{
	if (ft_redirect(mini, cmd) != 0)
		ft_exit_shell(mini, g_exit_code, NULL, 1);
	if (cmd->arg[0] && builtin_check(mini, cmd) == 0)
	{
		close_rdr_back(cmd);
		ft_exit_shell(mini, g_exit_code, NULL, 1);
	}
	else if (cmd->arg[0] && access(cmd->arg[0], X_OK) == 0)
		execute_in_dir(mini, cmd);
	else if (cmd->arg[0])
		execute_pathed_cmd(mini, cmd);
	close_rdr_back(cmd);
	ft_exit_shell(mini, g_exit_code, NULL, 2);
}

int	ft_fork(t_cmd *cmd, t_mini *mini)
{
	cmd->fork_id = fork();
	if (cmd->fork_id == -1)
	{
		fd_printf(2, "minishell: fork: %s\n", strerror(errno));
		wait_for_children(mini);
		g_exit_code = 1;
		return (-1);
	}
	mini++;
	return (cmd->fork_id);
}

void	execute_in_child(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->l_cmd;
	while (cmd)
	{
		if (ft_fork(cmd, mini) == -1)
			return ;
		if (cmd->fork_id == 0)
			exec_children_cmds(mini, cmd);
		cmd = cmd->next;
	}
	wait_for_children(mini);
}

void	parse_input(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->l_cmd;
	handle_heredoc(mini);
	if (is_parent_compatible(cmd))
		execute_in_parent(mini);
	else
		execute_in_child(mini);
}
