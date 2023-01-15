/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_token_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 18:48:47 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 18:49:56 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * @brief checks if the <string> is a redirection or not
 * 
 * @param string 
 * @return the an enumiration of the redirect 
 */
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

/**
 * @brief counts how many strings, excluding redirections and their files,
 * but including the NULL terminator.
 * 
 * @param token 
 * @return returns an int representing how many objects to be allocated
 */
int	ft_count_till_pipe(char **token)
{
	int	flag;
	int	count;

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

/**
 * @brief adds a node to the rdr linked list
 * 
 * @param rdr_head 
 * @param rdr_tail 
 * @param token 
 * @param i 
 * @return a pointer to the head of the rdr linked list
 */
t_rdr	*ft_add_to_rdrlist(t_rdr *rdr_head, t_rdr *rdr_tail,
							char **token, int *i)
{
	t_rdr	*rdr_new;

	rdr_new = (t_rdr *) ft_calloc(1, sizeof(t_rdr));
	if (!rdr_new)
		exit(1);
	rdr_new->e_rdr = ft_check_rdr(token[(*i)++]);
	rdr_new->file = ft_strdup(token[*i]);
	if (!rdr_new->file)
		exit(1);
	rdr_new->next = NULL;
	if (rdr_head == NULL)
		rdr_head = rdr_new;
	else
		rdr_tail->next = rdr_new;
	return (rdr_head);
}

/**
 * @brief iterates through the rdr linked list to fetch the tail.
 * 
 * @param rdr_head 
 * @return a pointer to the tail
 */
t_rdr	*ft_get_rdr_tail(t_rdr *rdr_head)
{
	t_rdr	*rdr_tail;

	rdr_tail = rdr_head;
	while (rdr_tail && rdr_tail->next)
		rdr_tail = rdr_tail->next;
	return (rdr_tail);
}

/**
 * @brief parses all the redirections and args in <token> into <cmd->arg>
 * and <cmd->rdr>
 * 
 * @param cmd 
 * @param token 
 * @param j counter for <cmd->arg>
 * @param i counter for <token>
 */
void	ft_populate_cmd(t_cmd *cmd, char **token, int *j, int *i)
{
	t_rdr	*rdr_tail;

	cmd->rdr = NULL;
	rdr_tail = NULL;
	while (token[*i] && ft_strncmp(token[*i], "|", 2))
	{
		if (ft_check_rdr(token[*i]))
		{
			cmd->rdr = ft_add_to_rdrlist(cmd->rdr, rdr_tail, token, i);
			rdr_tail = ft_get_rdr_tail(cmd->rdr);
		}
		else
		{
			cmd->arg[*j] = ft_strdup(token[*i]);
			if (!cmd->arg[*j])
				exit(1);
			(*j)++;
		}
		(*i)++;
	}
}
