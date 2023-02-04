/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/04 19:25:01 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exec_children_cmds(t_mini *mini, t_cmd *cmd)
{
	if (!cmd->arg[0])
		ft_exit_shell(mini, g_exit_code, NULL, 1);
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

void	execute_in_child(t_mini *mini)
{
	t_cmd	*cmd;
	t_cmd	*prevcmd;

	cmd = mini->l_cmd;
	prevcmd = NULL;
	while (cmd)
	{
		if (cmd->next != NULL && ft_pipe(cmd->fd_pipe, mini) == -1)
			return ;
		if (ft_fork(cmd, mini) == -1)
			return ;
		if (cmd->fork_id == 0 && cmd->next != NULL)
			dup_pipe_output(mini, cmd);
		if (cmd->fork_id == 0 && prevcmd != NULL)
			dup_pipe_input(mini, cmd, prevcmd);
		if (cmd->fork_id == 0)
			exec_children_cmds(mini, cmd);
		if (prevcmd)
			close_pipe(prevcmd);
		prevcmd = cmd;
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
