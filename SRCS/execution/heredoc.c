/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:27:57 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/05 19:25:32 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	null_params(char **tmp, char **total, char **line)
{
	*tmp = NULL;
	*total = NULL;
	*line = NULL;
}

bool	parse_delimiter(t_rdr *rdr)
{
	char	*tmp;
	char	hold;
	bool	quoted;

	quoted = false;
	tmp = rdr->file;
	while (*tmp)
		tmp++;
	while (tmp >= rdr->file)
	{
		while (tmp >= rdr->file && (*tmp != '\'' && *tmp != '\"'))
			tmp--;
		if (tmp < rdr->file)
			return (quoted);
		hold = *tmp;
		ft_memmove(tmp, tmp + 1, ft_strlen(tmp));
		ft_memmove(ft_strrchr(rdr->file, hold), \
		ft_strrchr(rdr->file, hold) + 1, \
		ft_strlen(ft_strrchr(rdr->file, hold)));
		quoted = true;
	}
	return (quoted);
}

void	trim_last_new_line(char *file)
{
	int	i;

	if (!file)
		return ;
	i = 0;
	while (file[i] != '\0')
		i++;
	if (file[i - 1] == '\n')
		file[i - 1] = '\0';
}

void	take_heredoc_input(t_rdr *rdr, t_mini *mini)
{
	char	*input;
	char	*total;
	char	*tmp;
	char	*line;
	bool	quoted;

	null_params(&tmp, &total, &line);
	quoted = parse_delimiter(rdr);
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_strncmp(input, rdr->file, ft_strlen(rdr->file) + 1) == 0)
			break ;
		tmp = line;
		line = gl_strjoin(input, "\n");
		if (!quoted)
			line = ft_check_var(line, mini);
		tmp = ft_free(tmp);
		tmp = total;
		total = gl_strjoin(total, line);
		line = ft_free(line);
		tmp = ft_free(tmp);
		input = ft_free(input);
	}
	input = ft_free(input);
	rdr->file = ft_free(rdr->file);
	if (!total)
		total = ft_strdup("");
	rdr->file = total;
	rdr->herepipe[0] = ft_close(rdr->herepipe[0], 3, NULL);
	fd_printf(rdr->herepipe[1], "%s", total);
	rdr->herepipe[1] = ft_close(rdr->herepipe[1], 3, NULL);
	ft_exit_shell(mini, 0, NULL, 2);
}

int	ft_pipe_heredoc(t_rdr *rdr, t_mini *mini, t_cmd *cmd)
{
	pipe(rdr->fdpipe);
	rdr->ret = 0;
	rdr->fork_id = fork();
	if (rdr->fork_id == 0)
	{
		rdr->fdpipe[0] = ft_close(rdr->fdpipe[0], 3, cmd);
		rdr->dup2_fd = dup2(rdr->fdpipe[1], STDOUT_FILENO);
		if (rdr->file)
			fd_printf(STDOUT_FILENO, "%s", rdr->file);
		rdr->fdpipe[1] = ft_close (rdr->fdpipe[1], 3, cmd);
		close_rdr_back(cmd);
		ft_exit_shell(mini, 0, NULL, 2);
	}
	else
	{
		rdr->fdpipe[1] = ft_close (rdr->fdpipe[1], 3, cmd);
		rdr->og_fd = dup(STDIN_FILENO);
		rdr->dup2_fd = dup2(rdr->fdpipe[0], STDIN_FILENO);
		waitpid(rdr->fork_id, &rdr->ret, 0);
		rdr->fdpipe[0] = ft_close (rdr->fdpipe[0], 3, cmd);
	}
	return (0);
}

void	read_heredoc_child(t_rdr *rdr)
{
	int		i;
	char	*buff;
	char	*str;
	char	*tmp;

	str = NULL;
	tmp = NULL;
	buff = ft_calloc(sizeof(char *), 2);
	i = 42;
	while (i)
	{
		i = read(rdr->herepipe[0], buff, 1);
		tmp = str;
		if (!buff[0])
			break ;
		str = gl_strjoin(tmp, buff);
		free(tmp);
	}
	rdr->file = ft_free(rdr->file);
	trim_last_new_line(str);
	rdr->file = str;
	buff = ft_free(buff);
}

void	handle_heredoc(t_mini *mini)
{
	t_cmd	*cmd;
	t_rdr	*rdr;
	int		status;

	status = 0;
	cmd = mini->l_cmd;
	while (cmd)
	{
		rdr = cmd->rdr;
		if (rdr)
		{
			while (rdr)
			{
				if (rdr->e_rdr == HEREDOC)
				{
					pipe(rdr->herepipe);
					rdr->fork_id = fork();
					if (rdr->fork_id == 0)
						take_heredoc_input(rdr, mini);
					else
					{
						ft_close(rdr->herepipe[1], 3, NULL);
						waitpid(rdr->fork_id, &status, 0);
						read_heredoc_child(rdr);
						ft_close(rdr->herepipe[0], 3, NULL);
					}
				}
				rdr = rdr->next;
			}
		}
		cmd = cmd->next;
	}
}
