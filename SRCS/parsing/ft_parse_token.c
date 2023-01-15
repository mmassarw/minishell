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
 * @brief initiates the variables in <cmd>
 * with the values parsed through <token>
 * 
 * @param cmd 
 * @param token 
 * @return The iteration count of <token>.
 */
int	ft_init_arg_n_rdr(t_cmd *cmd, char **token)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	cmd->arg = (char **) ft_calloc(ft_count_till_pipe(token), sizeof(char *));
	if (!cmd->arg)
		exit(1);
	ft_populate_cmd(cmd, token, &j, &i);
	return (i);
}

/**
 * @brief parses through <token>, allocates, and initializes a
 * t_cmd linked list.
 * 
 * @param token 
 * @return the t_cmd linked list.
 */
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
			exit(1);
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
