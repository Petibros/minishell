/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 03:10:42 by sacgarci          #+#    #+#             */
/*   Updated: 2025/01/07 20:29:03 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **array, int n)
{
	if (n > 0)
	{
		n--;
		while (n >= 0)
		{
			free(array[n]);
			n--;
		}
	}
	free(array);
}

static void	ft_strncpy(char *dest, char const *src, int n)
{
	int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static int	fill_array(char const *s, char c, char **array, int count)
{
	int	i;
	int	j;
	int	start;

	start = 0;
	j = 0;
	i = 0;
	while (j < count)
	{
		if (s[i] == c && s[i - 1] == c)
			start = -1;
		else if (s[i] == c || s[i] == '\0')
		{
			array[j] = malloc((start + 1) * sizeof(char));
			if (!array[j])
				return (j);
			ft_strncpy(array[j], &s[i - start], start);
			start = -1;
			j++;
		}
		i++;
		start++;
	}
	array[j] = NULL;
	return (-1);
}

static int	numstring(char const *s, char c)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c)
			count++;
		else if (s[i + 1] == '\0')
			count++;
		i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		count;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] == c && c != '\0')
		i++;
	count = numstring(&s[i], c);
	array = malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	count = fill_array(&s[i], c, array, count);
	if (count >= 0 && !array[count])
	{
		ft_free(array, count);
		return (NULL);
	}
	return (array);
}
