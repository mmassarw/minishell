/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 03:14:18 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 07:45:54 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_check_rdr(char *string)
{
	if (!ft_strncmp(string, "<", 2))
		return (INPUT);
	else if (!ft_strncmp(string, ">", 2))
		return (OUTPUT);
	else if (!ft_strncmp(string, "<<", 3))
		return (HEREDOC);
	else if (!ft_strncmp(string, ">>", 3))
		return (APPEND);
	else
		return (NONE);
}

int	ft_count_till_pipe(char **token)
{
	int	flag;
	int count;

	count = 0;
	flag = false;
	while (*token && ft_strncmp(*token, "|", 2))
	{
		if (!ft_check_rdr(*token))
		{
			if (flag == false)
				count++;
			flag = false;
		}
		else
			flag = true;
		token++;
		if (token == NULL)
			break ;
	}
	count++;
	return (count);
}

int	ft_init_arg_n_rdr(t_cmd *cmd, char **token)
{
	t_rdr	*rdr_new;
	t_rdr	*rdr_head;
	t_rdr	*rdr_tail;
	int		i;
	int		j;

	i = 0;
	j = 0;
	rdr_head = NULL;
	cmd->arg = (char **) ft_calloc(ft_count_till_pipe(token), 8);
	if (!cmd->arg)
		exit(1);	// change exit
	while (token[i] && ft_strncmp(token[i], "|", 2))
	{
		if (ft_check_rdr(token[i]))
		{
			rdr_new = (t_rdr *) ft_calloc(1, sizeof(t_rdr));
			if (!rdr_new)
				exit(1);	// change exit
			rdr_new->e_rdr = ft_check_rdr(token[i++]);
			rdr_new->file = ft_strdup(token[i]);
			if (!rdr_new->file)
				exit(1);	// change exit
			rdr_new->next = NULL;
			if (rdr_head == NULL)
				rdr_head = rdr_new;
			else
				rdr_tail->next = rdr_new;
			rdr_tail = rdr_new;
			cmd->rdr = rdr_head;
		}
		else
		{
			cmd->arg[j] = ft_strdup(token[i]);
			if (!cmd->arg[j])
				exit(1);	// change exit
			j++;
		}
		i++;
	}
	return (i);
}

t_cmd	*ft_parse_token(char **token)
{
	t_cmd	*cmd_new;
	t_cmd	*cmd_head;
	t_cmd	*cmd_tail;

	cmd_head = NULL;
	while (*token)
	{
		cmd_new = (t_cmd *) ft_calloc(1, sizeof(t_cmd));
		if (!cmd_new)
			exit(1);	// change exit
		token += ft_init_arg_n_rdr(cmd_new, token);
		cmd_new->next = NULL;
		if (cmd_head == NULL)
			cmd_head = cmd_new;
		else
			cmd_tail->next = cmd_new;
		cmd_tail = cmd_new;
		if (*token)
			if (!ft_strncmp(*token, "|", 2))
				token++;
	}
	return (cmd_head);
}