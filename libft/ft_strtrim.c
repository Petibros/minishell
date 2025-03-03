/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 22:55:35 by sacgarci          #+#    #+#             */
/*   Updated: 2024/10/11 02:57:38 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy(char *dest, char const *src, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	define_end(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = ft_strlen(s1) - 1;
	if (set[0] == '\0')
		return (i + 1);
	while (i > 0)
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			else if (s1[i] != set[j] && set[j + 1] == '\0')
				return (i + 1);
			j++;
		}
		i--;
	}
	return (i + 1);
}

static int	define_start(char const *s1, char const *set)
{
	int	i;
	int	j;

	if (set[0] == '\0')
		return (0);
	i = 0;
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			else if (s1[i] != set[j] && set[j + 1] == '\0')
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	int		start;
	int		end;

	if (!set || !s1)
		return (NULL);
	start = define_start(s1, set);
	end = define_end(&s1[start], set);
	trimmed = malloc((end + 1) * sizeof(char));
	if (!trimmed)
		return (NULL);
	return (ft_strncpy(trimmed, &s1[start], end));
}
