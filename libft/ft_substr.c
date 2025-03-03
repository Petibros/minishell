/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 03:26:14 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/14 22:56:11 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*subs;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	if (ft_strlen(s) <= start)
		subs = malloc(1 * sizeof(char));
	else if (ft_strlen(&s[start]) < len)
		subs = malloc(((ft_strlen(s) - start) + 1) * sizeof(char));
	else
		subs = malloc((len + 1) * sizeof(char));
	if (!subs)
		return (NULL);
	while (i < len && (start + i <= ft_strlen(s)))
	{
		subs[i] = s[start + i];
		i++;
	}
	subs[i] = '\0';
	return (subs);
}
