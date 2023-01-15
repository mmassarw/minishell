/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_parse_token.c								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmassarw <mmassarw@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/12 03:14:18 by mmassarw		  #+#	#+#			 */
/*   Updated: 2023/01/15 07:52:33 by mmassarw		 ###   ########.fr	   */
/*																			*/
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

/**
 * @brief allocates memory for <cmd->arg>
 * 
 * @param token 
 * @return the allocated memory as char **
 */
char **ft_create_arg_array(char **token)
{
	char **arg = (char **) ft_calloc(ft_count_till_pipe(token), 8);
	if (!arg)
		exit(1);	// change exit
	return arg;
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
t_rdr *ft_add_to_rdrlist(t_rdr *rdr_head, t_rdr *rdr_tail, char **token, int *i)
{
    t_rdr *rdr_new;

    rdr_new = (t_rdr *) ft_calloc(1, sizeof(t_rdr));
    if (!rdr_new)
        exit(1);  // change exit

    rdr_new->e_rdr = ft_check_rdr(token[(*i)++]);
    rdr_new->file = ft_strdup(token[*i]);
    if (!rdr_new->file)
        exit(1);  // change exit
    rdr_new->next = NULL;

    if (rdr_head == NULL)
        rdr_head = rdr_new;
    else
        rdr_tail->next = rdr_new;

    return rdr_head;
}

/**
 * @brief iterates through the rdr linked list to fetch the tail.
 * 
 * @param rdr_head 
 * @return a pointer to the tail
 */
t_rdr *ft_get_rdr_tail(t_rdr *rdr_head)
{
    t_rdr *rdr_tail = rdr_head;

    while (rdr_tail && rdr_tail->next)
        rdr_tail = rdr_tail->next;

    return rdr_tail;
}

t_rdr *ft_populate_arg_array(t_cmd *cmd, char **token, int *j, int *i)
{
    t_rdr *rdr_head;
    t_rdr *rdr_tail;

    rdr_head = NULL;
    rdr_tail = NULL;
	while (token[*i] && ft_strncmp(token[*i], "|", 2))
	{
        if (ft_check_rdr(token[*i]))
        {
            rdr_head = ft_add_to_rdrlist(rdr_head, rdr_tail, token, i);
            rdr_tail = ft_get_rdr_tail(rdr_head);
        }
		else
		{
			cmd->arg[*j] = ft_strdup(token[*i]);
			if (!cmd->arg[*j])
				exit(1);	// change exit
			(*j)++;
		}
		(*i)++;
	}
	return (rdr_head);
}

int ft_init_arg_n_rdr(t_cmd *cmd, char **token)
{
	int i;
	int j;

	i = 0;
	j = 0;
	cmd->arg = ft_create_arg_array(token);
	cmd->rdr = ft_populate_arg_array(cmd, token, &j, &i);
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
