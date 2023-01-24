/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/25 00:29:33 by hakaddou         ###   ########.fr       */
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

void	close_rdr_files(t_mini *mini, t_cmd *cmd)
{
	t_rdr	*rdr;
	int		fd;

	rdr = cmd->rdr;
	while (rdr)
	{
		if (rdr->fd > 2)
		{
			fd = close(rdr->fd);
			if (fd != 0)
				ft_exit_shell(mini, errno, strerror(errno), 2);
			else
				rdr->fd = -2;
		}
		rdr = rdr->next;
	}
}

int	error_set_print_close(t_mini *mini, t_cmd *cmd, int error)
{
	g_exit_code = error;
	fd_printf (2, "minishell: %s\n", strerror(errno));
	close_rdr_files(mini, cmd);
	return (1);
}

int	parse_redirect(t_mini *mini, t_cmd *cmd)
{
	t_rdr	*rdr;
	int 	fd;

	rdr = cmd->rdr;
	while (rdr->next)
	{
		if (rdr->fd > 2)	
		{
			fd = close(rdr->fd);
			if (fd != 0)
				ft_exit_shell(mini, errno, strerror(errno), 2);
		}
		rdr = rdr->next;
	}
	if (rdr->fd > 2)
	{
		fd = close(rdr->fd);
		if (fd != 0)
			ft_exit_shell(mini, errno, strerror(errno), 2);
	}
	rdr->og_fd = dup(STDOUT_FILENO);
	rdr->fd = open(rdr->file, O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (rdr->fd == -1 || rdr->og_fd == -1)
	{
		fd_printf(2, "minishell: %s: %s\n", rdr->file, strerror(errno));
		return (1);
	}
	rdr->dup2_fd = dup2(rdr->fd, STDOUT_FILENO);
	return (0);
}

int	file_no_exist(t_mini *mini, t_rdr *trdr)
{
	t_rdr *rdr;
	int	c_fd;

	rdr = trdr;
	if (rdr->e_rdr == OUTPUT)
	{
		rdr->fd = open(rdr->file, O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (rdr->fd == -1)
			ft_exit_shell(mini, errno, strerror(errno), 2);
		c_fd = close (rdr->fd);
		if (c_fd == -1)
			ft_exit_shell(mini, errno, strerror(errno), 2);
		rdr->fd = -1;
	}
	else if (rdr->e_rdr == APPEND)
	{
		rdr->fd = open(rdr->file, O_RDWR | O_APPEND | O_CREAT, 0644);
		if (rdr->fd == -1)
			ft_exit_shell(mini, errno, strerror(errno), 2);
		c_fd = close (rdr->fd);
		if (c_fd == -1)
			ft_exit_shell(mini, errno, strerror(errno), 2);
		rdr->fd = -1;
	}
	else if (rdr->e_rdr == INPUT)
		return (1);
	return (0);
}

int	ft_redirect(t_mini *mini, t_cmd *cmd)
{
	t_rdr	*rdr;
	int		flag;

	flag = 0;
	rdr = cmd->rdr;
	if (rdr == NULL)
		return (0);
	while (rdr != NULL && flag == 0)
	{
		// checks if file exists
		if (access(rdr->file, F_OK) != EXIST)
			flag = file_no_exist(mini, rdr); // if file doesn't exists it creates it if it's output otherwise checks read rights on input
		else if (access(rdr->file, W_OK) != 0)
			flag = 1;
		else
		{
			rdr->fd = open(rdr->file, O_RDWR | O_TRUNC, 0644);
			if (rdr->fd == -1)
				ft_exit_shell(mini, errno, strerror(errno), 2);
			rdr->fd = close (rdr->fd);
			if (rdr->fd == -1)
				ft_exit_shell(mini, errno, strerror(errno), 2);
		}
		rdr = rdr->next;
	}
	if (flag != 0)
		return (error_set_print_close(mini, cmd, 1));
	else
		return (parse_redirect(mini, cmd));
}

void	close_rdr_back(t_cmd *cmd)
{
	t_rdr *rdr;

	rdr = cmd->rdr;
	if (!cmd || !cmd->rdr)
		return ;
	while (rdr->next)
		rdr = rdr->next;
	close(rdr->dup2_fd);
	dup2(rdr->og_fd, STDOUT_FILENO);
	close(rdr->og_fd);
	close(rdr->fd);
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
		{
			cmd = cmd->next;
			continue ;
		}
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
