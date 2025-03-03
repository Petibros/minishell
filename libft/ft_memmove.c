/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:34:20 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/14 23:42:51 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;
	long	j;

	i = 0;
	if (dest == src || n == 0)
		return (dest);
	if (dest > src)
	{
		j = n - 1;
		while (j >= 0)
		{
			(((char *)dest)[j]) = (((char *)src)[j]);
			j--;
		}
		return (dest);
	}
	while (i < n)
	{
		(((char *)dest)[i]) = (((char *)src)[i]);
		i++;
	}
	return (dest);
}
