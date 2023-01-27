/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/27 03:59:17 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	execute_in_dir(t_mini *mini, t_cmd *cmd)
{
	int		id;
	char	**envc;

	if (dot_dir_check(cmd))
		return ;
	id = fork();
	if (id == 0)
	{
		envc = convert_env(mini);
		if (execve(cmd->arg[0], cmd->arg, envc) == -1)
		{
			ft_free_split(envc);
			ft_exit_shell(mini, errno, strerror(errno), 2);
		}
	}
	else
	{
		wait(NULL);
		if (errno == SUCCESS)
			set_env_underscore(cmd->arg[0] + 2, mini);
		g_exit_code = errno;
	}
}

void	command_failed_message(t_cmd *cmd, int code)
{
	g_exit_code = code;
	fd_printf(2, RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
		cmd->arg[0]);
}

void	execute_command_fork(t_mini *mini, t_cmd *cmd, char *cmd_path)
{
	int		id;
	char	**envc;

	set_env_underscore(cmd->arg[0], mini);
	id = fork();
	if (id == 0)
	{
		envc = convert_env(mini);
		if (execve(cmd_path, cmd->arg, envc) == -1)
		{
			ft_free_split(envc);
			ft_exit_shell(mini, errno, strerror(errno), 2);
		}
	}
	else
	{
		g_exit_code = SUCCESS;
		wait (NULL);
	}
	free (cmd_path);
}

void	execute_pathed_cmd(t_mini *mini, t_cmd *cmd)
{
	char	*cmd_path;

	if (ft_strchr(cmd->arg[0], '.') && ft_strchr(cmd->arg[0], '/'))
	{
		if (!file_exists(cmd->arg[0]))
		{
			fd_printf(2, "execute_pathed_cmd: minishell:");
			fd_printf(2, "%s: No such file or directory\n", cmd->arg[0]);
			g_exit_code = 127;
			return ;
		}
	}
	if (find_str_env("PATH", mini, KEY) == NULL)
		return (command_failed_message(cmd, COMMAND_FAIL));
	cmd_path = get_path(cmd->arg[0], find_str_env("PATH", mini, VALUE));
	if (cmd_path)
		execute_command_fork(mini, cmd, cmd_path);
	else
		command_failed_message(cmd, COMMAND_FAIL);
}

void	parse_input(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->l_cmd;
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
