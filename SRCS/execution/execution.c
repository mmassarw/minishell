/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/03 13:40:57 by hakaddou         ###   ########.fr       */
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
		g_exit_code = 1;
		wait_for_children(mini);
		return (-1);
	}
	return (cmd->fork_id);
}

// void	execute_in_child(t_mini *mini)
// {
// 	t_cmd	*cmd;

// 	cmd = mini->l_cmd;
// 	while (cmd)
// 	{
// 		if (ft_fork(cmd, mini) == -1)
// 			return ;
// 		if (cmd->fork_id == 0)
// 			exec_children_cmds(mini, cmd);
// 		cmd = cmd->next;
// 	}
// 	wait_for_children(mini);
// }

void	execute_in_child(t_mini *mini)
{
	t_cmd	*cmd;
	t_cmd	*prevcmd;

	cmd = mini->l_cmd;
	prevcmd = NULL;
	while (cmd)
	{
		// it will not open a pipe for the last comd, we only need the pipe of the previous one
		if (cmd->next != NULL)
			pipe(cmd->fd_pipe);
		if (ft_fork(cmd, mini) == -1)
			return ;
		if (cmd->fork_id == 0 && cmd->next != NULL)
		{
			// closing the current cmds input because we take input from the prev cmd pipe
			cmd->fd_pipe[0] = ft_close(cmd->fd_pipe[0], 3, cmd);
			// duping output for current cmd to the write end of the pipe
			cmd->odup2_fd = dup2(cmd->fd_pipe[1], STDOUT_FILENO);
			cmd->fd_pipe[1] = ft_close(cmd->fd_pipe[1], 0, cmd);
		}
		if (cmd->fork_id == 0 && prevcmd != NULL)
		{
			// closing the prev command output in this child process (check if it causes errors)
			prevcmd->fd_pipe[1] = ft_close(prevcmd->fd_pipe[1], 3, cmd);
			cmd->idup2_fd = dup2(prevcmd->fd_pipe[0], STDIN_FILENO);
			cmd->fd_pipe[0] = ft_close(cmd->fd_pipe[0], 0, cmd);
		}
		if (prevcmd)
		{
			prevcmd->fd_pipe[0] = ft_close (prevcmd->fd_pipe[0], 0, cmd);
			prevcmd->fd_pipe[1] = ft_close (prevcmd->fd_pipe[1], 0, cmd);
		}
		if (cmd->fork_id == 0)
			exec_children_cmds(mini, cmd);
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
