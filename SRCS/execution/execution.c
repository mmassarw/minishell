/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 00:41:59 by hakaddou          #+#    #+#             */
/*   Updated: 2023/01/29 17:04:17 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*gl_strchr(const char *s, int c)
{
	char	*str;

	if (!s)
		return (0);
	str = (char *)s;
	while (*str != c)
	{
		if (*str == '\0')
			return (NULL);
		str++;
	}
	return (str);
}

int	gl_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*gl_strjoin(char *s1, char *s2)
{
	char	*str;
	int		len1;
	int		len2;
	int		i;

	i = 0;
	len1 = gl_strlen(s1);
	len2 = gl_strlen(s2);
	if (!len1 && !len2)
		return (0);
	str = ft_calloc(len1 + len2 + 1, 1);
	if (!str)
		return (0);
	str[0] = '\0';
	while (i < len1 + len2)
	{
		if (i < len1)
			str[i] = s1[i];
		if (i >= len1)
			str[i] = s2[i - len1];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	null_params(char **tmp, char **total, char **line)
{
	*tmp = NULL;
	*total = NULL;
	*line = NULL;
}

void	take_heredoc_input(t_rdr *rdr)
{
	char	*input;
	char	*total;
	char	*tmp;
	char	*line;

	null_params(&tmp, &total, &line);
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_strncmp(input, rdr->file, ft_strlen(rdr->file) + 1) == 0)
			break ;
		tmp = line;
		line = gl_strjoin(input, "\n");
		tmp = ft_free(tmp);
		tmp = total;
		total = gl_strjoin(total, line);
		line = ft_free(line);
		tmp = ft_free(tmp);
		input = ft_free(input);
	}
	input = ft_free(input);
	rdr->file = ft_free(rdr->file);
	rdr->file = total;
}

void	close_all_fds(t_mini *mini)
{
	t_cmd	*cmd;
	t_rdr	*rdr;

	cmd = mini->l_cmd;
	while (cmd)
	{
		rdr = cmd->rdr;
		if (rdr)
		{
			while (rdr)
			{
				if (rdr->dup2_fd >= 0)
				{
					close (rdr->dup2_fd);
					rdr->dup2_fd = -2;
				}
				if (rdr->fd >= 0)
				{
					close (rdr->fd);
					rdr->fd = -2;
				}
				if (rdr->fdpipe[0] >= 0)
				{
					close (rdr->fdpipe[0]);
					rdr->fdpipe[0] = -2;
				}
				if (rdr->fdpipe[1] >= 0)
				{
					close (rdr->fdpipe[1]);
					rdr->fdpipe[1] = -2;
				}
				if (rdr->og_fd >= 0)
				{
					close (rdr->og_fd);
					rdr->og_fd = -2;
				}
				rdr = rdr->next;
			}
			
		}
		cmd = cmd->next;
	}
}

int	ft_pipe_heredoc(t_rdr *rdr, t_mini *mini, t_cmd *cmd)
{
	
	pipe(rdr->fdpipe);
	rdr->ret = 0;
	rdr->fork_id = fork();
	if (rdr->fork_id == 0)
	{
		close(rdr->fdpipe[0]);
		rdr->fdpipe[0] = -2;
		rdr->dup2_fd = dup2(rdr->fdpipe[1], STDOUT_FILENO);
		if (rdr->file)
			fd_printf(STDOUT_FILENO, "%s", rdr->file);
		close (rdr->fdpipe[1]);
		rdr->fdpipe[1] = -2;
		close_rdr_back(cmd);
		ft_exit_shell(mini, 0, NULL, 2);
	}
	else
	{
		if (rdr->fdpipe[1] >= 0)
		{
			close (rdr->fdpipe[1]);
			rdr->fdpipe[1] = -2;
		}
		rdr->og_fd = dup(STDIN_FILENO);
		rdr->dup2_fd = dup2(rdr->fdpipe[0], STDIN_FILENO);
		waitpid(rdr->fork_id, &rdr->ret, WEXITSTATUS(rdr->ret));
	}
	return (0);
}

void	handle_heredoc(t_mini *mini)
{
	t_cmd	*cmd;
	t_rdr	*rdr;

	cmd = mini->l_cmd;
	while (cmd)
	{
		rdr = cmd->rdr;
		if (rdr)
		{
			while (rdr)
			{
				if (rdr->e_rdr == HEREDOC)
					take_heredoc_input(rdr);
				rdr = rdr->next;
			}
		}
		cmd = cmd->next;
	}
}

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
