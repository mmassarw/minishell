/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 15:41:18 by mmassarw          #+#    #+#             */
/*   Updated: 2023/01/07 02:53:17 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates enough space for <nmemb> objects
 * that are <size> bytes of memory each
 * and returns a pointer to the allocated memory.
 * The allocated memory is filled with bytes of value zero.
 * 
 * @param nmemb
 * @param size
 * @return If successful, return a pointer to allocated memory,
 * @return if there is an error, return a NULL pointer.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;

	if (nmemb >= SIZE_MAX || size >= SIZE_MAX)
		return (NULL);
	p = malloc(nmemb * size);
	if (!p)
		return (NULL);
	ft_bzero(p, (nmemb * size));
	return (p);
}
