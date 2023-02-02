/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/02 21:11:43 by hakaddou         ###   ########.fr       */
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
// 	int		i;

// 	i = 0;
// 	cmd = mini->l_cmd;
// 	// if (cmd->next)
// 	while (cmd)
// 	{
// 		pipe(cmd->ifd_pipe);
// 		pipe(cmd->ofd_pipe);
// 		if (i != 0)
// 		{
// 			cmd->ifd_pipe[1] = ft_close(cmd->ifd_pipe[1], 3, cmd);
// 			cmd->i_fd = dup(STDIN_FILENO);
// 			cmd->idup2_fd = dup2(cmd->ifd_pipe[0], STDIN_FILENO);
// 			// cmd->ifd_pipe[0] = ft_close(cmd->ifd_pipe[0], 3, cmd);
// 		}
// 		if (cmd->next != NULL)
// 		{
// 			cmd->ofd_pipe[0] = ft_close(cmd->ofd_pipe[0], 3, cmd);
// 			cmd->o_fd = dup(STDOUT_FILENO);
// 			cmd->odup2_fd = dup2(cmd->ofd_pipe[1], STDOUT_FILENO);
// 			// cmd->ofd_pipe[1] = ft_close(cmd->ofd_pipe[1], 3, cmd);
// 		}
// 		if (ft_fork(cmd, mini) == -1)
// 			return ;
// 		if (cmd->fork_id == 0)
// 			exec_children_cmds(mini, cmd);
// 		cmd = cmd->next;
// 		i++;
// 	}
// 	cmd = mini->l_cmd;
// 	i = 0;
// 	while (cmd)
// 	{
// 		if (cmd->next != NULL)
// 		{
// 			cmd->odup2_fd = ft_close (cmd->odup2_fd, 3, cmd);
// 			dup2(cmd->o_fd, STDOUT_FILENO);
// 			cmd->o_fd = ft_close(cmd->o_fd, 3, cmd);
// 			cmd->ofd_pipe[1] = ft_close(cmd->ofd_pipe[1], 3, cmd);
// 			cmd->ofd_pipe[0] = ft_close(cmd->ofd_pipe[0], 3, cmd);
// 		}
// 		if (i != 0)
// 		{
// 			cmd->idup2_fd = ft_close(cmd->idup2_fd, 3, cmd);
// 			dup2(cmd->i_fd, STDIN_FILENO);
// 			cmd->i_fd = ft_close(cmd->i_fd, 3, cmd);
// 			cmd->ifd_pipe[0] = ft_close(cmd->ifd_pipe[0], 3, cmd);
// 			cmd->ifd_pipe[1] = ft_close(cmd->ifd_pipe[1], 3, cmd);
// 		}
// 		i++;
// 		cmd = cmd->next;
// 	}
// 	wait_for_children(mini);
// }

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
			// cmd->o_fd = dup(STDOUT_FILENO);
			// duping output for current cmd to the write end of the pipe
			cmd->odup2_fd = dup2(cmd->fd_pipe[1], STDOUT_FILENO);
		}
		if (cmd->fork_id == 0 && prevcmd != NULL)
		{
			// closing the prev command output in this child process (check if it causes errors)
			prevcmd->fd_pipe[1] = ft_close(prevcmd->fd_pipe[1], 3, cmd);
			// cmd->i_fd = dup(STDIN_FILENO);
			cmd->idup2_fd = dup2(prevcmd->fd_pipe[0], STDIN_FILENO);
		}
		if (cmd->fork_id == 0)
			exec_children_cmds(mini, cmd);
		if (prevcmd)
		{
			close (prevcmd->fd_pipe[0]);
			close (prevcmd->fd_pipe[1]);
		}
		prevcmd = cmd;
		cmd = cmd->next;
	}
	// prevcmd->idup2_fd = ft_close(prevcmd->idup2_fd, 0, cmd);
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
