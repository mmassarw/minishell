/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 05:56:17 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/15 18:40:17 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Frees all allocated memory within the double pointer <split>
 * including itself.
 * 
 * @param split 
 */
void	*ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		split[i] = (char *) ft_free(split[i]);
		i++;
	}
	split = (char **) ft_free(split);
	return (NULL);
}
