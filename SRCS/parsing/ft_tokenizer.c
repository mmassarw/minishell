/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 23:12:20 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/26 22:01:24 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*ft_next_token(t_mini *mini, t_token *new, size_t offset)
{
}

void	ft_tokenlist(t_mini *mini)
{
	t_token *new;
	t_token	*prev;
	size_t	offset;

	offset = 0;
	prev = NULL;
	while(*(mini->rl + offset))
	{
		new = (t_token *) ft_calloc(1, sizeof(t_token));
		if (!new)
			ft_exit_shell(mini, 137, "Page allocation failure\n", 2);
		ft_next_token(mini, new, offset);
		if (prev)
			prev->next = new;
		if (!mini->l_token)
			mini->l_token = new;
		prev = new;
	}
}

void	ft_tokenize(t_mini *mini)
{
	// assigne token type
	ft_tokenlist(mini);

	// expand variable
	
	// collapse quotes
	
	// convert linked list to array of strings
}