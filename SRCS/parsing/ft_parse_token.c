/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 03:14:18 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/13 02:04:46 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_till_pipe(char **token)
{
	int	i;

	i = 0;
	while (token[i] || !ft_strnstr(token[i], "|", 1))
		if (ft_check_rdr(*token))
			i++;
	return (i);
}

t_rdr	*ft_check_rdr(char *string)
{
	if (ft_strnstr(string, "<", 1))
		return (INPUT);
	else if (ft_strnstr(string, ">", 1))
		return (OUTPUT);
	else if (ft_strnstr(string, "<<", 2))
		return (HEREDOC);
	else if (ft_strnstr(string, ">>", 2))
		return (OUTPUT);
	else
		return (NONE);
}

void	ft_init_arg_n_rdr(t_cmd *cmd, char **token)
{
	t_rdr	*rdr_new;
	t_rdr	*rdr_head;
	t_rdr	*rdr_tail;

}

t_cmd	*ft_init_t_cmd(char **token)
{
	t_cmd	*cmd_new;
	t_cmd	*cmd_head;
	t_cmd	*cmd_tail;

	cmd_head = NULL;
	cmd_tail = NULL;
	while (*token)
	{
		cmd_new = (t_cmd *) ft_calloc(1, sizeof(t_cmd));
		// if (!cmd_new)
		// 	error
		// init cmd
		cmd_new->next = NULL;
		if (cmd_head == NULL)
			cmd_head = cmd_new;
		else
			cmd_tail->next = cmd_new;
		cmd_tail = cmd_new;
	}
	return (cmd_head);
}
