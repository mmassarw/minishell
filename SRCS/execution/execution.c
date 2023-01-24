/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/22 21:08:25 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// parse_input expects clean input from l_cmd->arg[0]
// some args are being free like the echo args as
// they're being split, after merging, the freeing should be removed
// and replaced with a speacial freeing function

void	execute_in_dir(t_mini *mini, t_cmd *cmd)
{
	int		id;
	char	**envc;

	if (cmd->arg[0][2] == '\0')
	{
		fd_printf(2, "bash: ./: is a directory\n");
		g_exit_code = 126;
		return ;
	}
	id = fork();
	if (id == 0)
	{
		envc = convert_env(mini);
		if (execve(cmd->arg[0], cmd->arg, envc) == -1)
		{
			fd_printf(2, "minishell: %s\n", strerror(errno));
			ft_free_split(envc);
			g_exit_code = errno;
			exit (errno);
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
			fd_printf(2, "minishell: %s\n", strerror(errno));
			ft_free_split(envc);
			g_exit_code = errno;
			exit (errno);
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

	if (find_str_env("PATH", mini, KEY) == NULL)
	{
		command_failed_message(cmd, COMMAND_FAIL);
		return ;
	}
	cmd_path = get_path(cmd->arg[0], find_str_env("PATH", mini, VALUE));
	if (cmd_path)
		execute_command_fork(mini, cmd, cmd_path);
	else
		command_failed_message(cmd, COMMAND_FAIL);
}

int	ft_redirect(t_mini *mini, t_cmd *cmd)
{
	t_rdr	*rdr;
	// int		r_fd;
	
	rdr = cmd->rdr;
	if (rdr == NULL)
		return (0);
	while (rdr != NULL)
	{
		rdr->og_fd = dup(STDOUT_FILENO);
		rdr->fd = open(rdr->file, O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (rdr->fd == -1 || rdr->og_fd == -1)
		{
			fd_printf(2, "minishell: %s: %s\n", rdr->file, strerror(errno));
			return (1);
		}
		rdr->dup2_fd = dup2(rdr->fd, STDOUT_FILENO);
		rdr = rdr->next;
	}
	mini++;
	return (0);
}

void	close_rdr_back(t_cmd *cmd)
{
	if (!cmd || !cmd->rdr)
		return ;
	close(cmd->rdr->fd);
	close(cmd->rdr->dup2_fd);
	dup2(cmd->rdr->og_fd, STDOUT_FILENO);
	close(cmd->rdr->og_fd);
}

void	parse_input(t_mini *mini)
{
	t_cmd	*cmd;

	cmd = mini->l_cmd;
	while (cmd)
	{
		if (!cmd->arg[0][0])
			continue ;
		if (ft_redirect(mini, cmd) != 0)
			return ;
		if (builtin_check(mini, cmd) == 0)
		{
			close_rdr_back(cmd);
			cmd = cmd->next;
			continue ;
		}
		else if (!ft_strncmp(cmd->arg[0], "./", 2)
			&& !access(cmd->arg[0], X_OK))
			execute_in_dir(mini, cmd);
		else
			execute_pathed_cmd(mini, cmd);
		close_rdr_back(cmd);
		cmd = cmd->next;
	}
}
