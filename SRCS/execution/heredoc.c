/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 19:27:57 by hakaddou          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/02/05 21:06:00 by hakaddou         ###   ########.fr       */
=======
/*   Updated: 2023/02/05 19:25:32 by mmassarw         ###   ########.fr       */
>>>>>>> origin/mmassarw
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	null_params(char **tmp, char **total, char **line)
{
	*tmp = NULL;
	*total = NULL;
	*line = NULL;
}

void	free_params(char **line, char **tmp, char **input)
{
	*line = ft_free(*line);
	*tmp = ft_free(*tmp);
	*input = ft_free(*input);
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
		free_params(&line, &tmp, &input);
	}
	send_input_to_parent(mini, input, rdr, total);
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
					if (take_input_from_child(mini, rdr) == -1)
						return ;
				rdr = rdr->next;
			}
		}
		cmd = cmd->next;
	}
}
