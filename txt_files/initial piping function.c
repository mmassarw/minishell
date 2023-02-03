/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial piping function.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:21:58 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/03 20:22:00 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	original function without piping	*/
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

/*	piping function with comments	*/
// void	execute_in_child(t_mini *mini)
// {
// 	t_cmd	*cmd;
// 	t_cmd	*prevcmd;

// 	cmd = mini->l_cmd;
// 	prevcmd = NULL;
// 	while (cmd)
// 	{
// 		/*it will not open a pipe for the last comd, we
// 		only need the pipe of the previous one*/
// 		if (cmd->next != NULL)
// 			pipe(cmd->fd_pipe);
// 		if (ft_fork(cmd, mini) == -1)
// 			return ;
// 		if (cmd->fork_id == 0 && cmd->next != NULL)
// 		{
// 			/*closing the current cmds input because
// 			we take input from the prev cmd pipe*/
// 			cmd->fd_pipe[0] = ft_close(cmd->fd_pipe[0], 3, NULL);
// 			// duping output for current cmd to the write end of the pipe
// 			cmd->odup2_fd = dup2(cmd->fd_pipe[1], STDOUT_FILENO);
// 			cmd->fd_pipe[1] = ft_close(cmd->fd_pipe[1], 0, NULL);
// 		}
// 		if (cmd->fork_id == 0 && prevcmd != NULL)
// 		{
// 			/*closing the prev command output in this
// 			child process (check if it causes errors)*/
// 			prevcmd->fd_pipe[1] = ft_close(prevcmd->fd_pipe[1], 3, cmd);
// 			cmd->idup2_fd = dup2(prevcmd->fd_pipe[0], STDIN_FILENO);
// 			cmd->fd_pipe[0] = ft_close(cmd->fd_pipe[0], 0, cmd);
// 		}
// 		if (prevcmd)
// 		{
// 			prevcmd->fd_pipe[0] = ft_close (prevcmd->fd_pipe[0], 0, cmd);
// 			prevcmd->fd_pipe[1] = ft_close (prevcmd->fd_pipe[1], 0, cmd);
// 		}
// 		if (cmd->fork_id == 0)
// 			exec_children_cmds(mini, cmd);
// 		prevcmd = cmd;
// 		cmd = cmd->next;
// 	}
// 	wait_for_children(mini);
// }
