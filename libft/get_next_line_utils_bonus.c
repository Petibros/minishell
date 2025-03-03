/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:50:36 by sacgarci          #+#    #+#             */
/*   Updated: 2025/01/08 03:11:48 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*res;
	int		size;	
	int		i;

	size = ft_strnlen(s1, '\0') + ft_strnlen(s2, '\0');
	res = ft_calloc_gnl((1 + size), sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		res[i + ft_strnlen(s1, '\0')] = s2[i];
		i++;
	}
	return (res);
}

char	*ft_calloc_gnl(int nmemb, int size)
{
	int		i;
	char	*str;

	str = malloc(nmemb * size);
	if (!str)
		return (NULL);
	i = 0;
	while (i < (nmemb * size))
	{
		str[i] = '\0';
		i++;
	}
	return (str);
}

int	ft_strnlen(char *str, char c)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

int	ft_strchr_gnl(char *src, char c)
{
	int	i;

	if (!src)
		return (0);
	i = 0;
	while (src[i])
	{
		if (src[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	ft_strcpy(char *dest, char *src)
{
	int		i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}
