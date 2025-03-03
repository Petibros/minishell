/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:09:08 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/09 16:40:50 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	lendst;

	if (dst == NULL && size == 0)
		return (ft_strlen(src));
	lendst = ft_strlen(dst);
	j = 0;
	i = lendst;
	if (size > 0 && size > lendst)
	{
		while (src[j] && j < (size - lendst - 1))
		{
			dst[i + j] = src[j];
			j++;
		}
		dst[i + j] = '\0';
		return (ft_strlen(src) + lendst);
	}
	return (size + ft_strlen(src));
}
