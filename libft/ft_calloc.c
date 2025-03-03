/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:18:18 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/11 23:31:32 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	i;

	i = 0;
	if (size == 0)
		return (malloc(0));
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	while (i < (nmemb * size))
	{
		(((char *)ptr)[i]) = '\0';
		i++;
	}
	return (ptr);
}
