/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:02:49 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/07 05:00:20 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates a new string,
 * which is the result of the concatenation of ’s1’ and ’s2’.
 * 
 * @param s1 
 * @param s2 
 * @return the joined string.
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	char	*p;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	p = (char *) malloc(len1 + len2 + 1);
	if (!p)
		return (NULL);
	ft_strlcpy(p, s1, len1 + 1);
	ft_strlcpy((p + len1), s2, len2 + 1);
	return (p);
}
