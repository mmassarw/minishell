/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/20 23:51:43 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// parse_input expects clean input from l_cmd->arg[0]
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function

void	execute_in_dir(t_mini *mini)
{
	int		id;
	char	**envc;

	if (mini->l_cmd->arg[0][2] == '\0')
	{
		fd_printf(2, "bash: ./: is a directory\n");
		return ;
	}
	set_env_underscore(mini->l_cmd->arg[0] + 2, mini);
	id = fork();
	if (id == 0)
	{
		envc = convert_env(mini);
		if (execve(mini->l_cmd->arg[0], mini->l_cmd->arg, envc) == -1)
		{
			fd_printf(2, "minishell: %s\n", strerror(errno));
			ft_free_split(envc);
			g_exit_code = errno;
			exit (errno);
		}
	}
	else
		wait(NULL);
}

void	command_failed_message(t_mini *mini, int code)
{
	g_exit_code = code;
	fd_printf(2, RED_FONT "minishell:  command not found "RESET_FONT "%s\n",
		mini->l_cmd->arg[0]);
}

void	execute_command_fork(t_mini *mini, char *cmd_path)
{
	int		id;
	char	**envc;

	set_env_underscore(mini->l_cmd->arg[0], mini);
	id = fork();
	if (id == 0)
	{
		envc = convert_env(mini);
		if (execve(cmd_path, mini->l_cmd->arg, envc) == -1)
		{
			fd_printf(2, "minishell: %s\n", strerror(errno));
			ft_free_split(envc);
			g_exit_code = errno;
			exit (errno);
		}
	}
	else
		wait (NULL);
	free (cmd_path);
}

void	execute_pathed_cmd(t_mini *mini)
{
	char	*cmd_path;

	if (find_str_env("PATH", mini, KEY) == NULL)
	{
		command_failed_message(mini, COMMAND_FAIL);
		return ;
	}
	cmd_path = get_path(mini->l_cmd->arg[0], find_str_env("PATH", mini, VALUE));
	if (cmd_path)
		execute_command_fork(mini, cmd_path);
	else
		command_failed_message(mini, COMMAND_FAIL);
}

void	parse_input(t_mini *mini)
{
	if (builtin_check(mini) == 0)
		return ;
	if (!mini->l_cmd->arg[0][0])
		return ;
	else if (!ft_strncmp(mini->l_cmd->arg[0], "./", 2)
		&& !access(mini->l_cmd->arg[0], X_OK))
		execute_in_dir(mini);
	else
		execute_pathed_cmd(mini);
}
