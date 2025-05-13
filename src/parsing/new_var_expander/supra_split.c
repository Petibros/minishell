/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supra_split.c                                      :+:      :+:    :+:   */
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

static int	is_separator(char c, char *seps)
{
	int	i;

	i = 0;
	while (seps[i])
	{
		if (c == seps[i])
			return (1);
		i++;
	}
	return (0);
}

static int	is_in_quotes(const char *s, int pos)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (i < pos)
	{
		if (s[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (s[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

static int	fill_array(char const *s, char *seps, char **array, int count)
{
	int	i;
	int	j;
	int	start;

	start = 0;
	j = 0;
	i = 0;
	while (j < count)
	{
		if (is_separator(s[i], seps) && !is_in_quotes(s, i) && is_separator(s[i - 1], seps))
			start = -1;
		else if ((is_separator(s[i], seps) && !is_in_quotes(s, i)) || s[i] == '\0')
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

static int	numstring(char const *s, char *seps)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (is_separator(s[i], seps) && !is_in_quotes(s, i) 
			&& (i == 0 || !is_separator(s[i - 1], seps) || s[i + 1] == '\0'))
			count++;
		else if (s[i + 1] == '\0' && !is_separator(s[i], seps))
			count++;
		i++;
	}
	return (count);
}

char	**supra_split(char const *s, char *seps)
{
	char	**array;
	int		count;
	int		i;

	if (!s || !seps)
		return (NULL);
	i = 0;
	while (s[i] && is_separator(s[i], seps) && !is_in_quotes(s, i))
		i++;
	count = numstring(&s[i], seps);
	array = malloc((count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	count = fill_array(&s[i], seps, array, count);
	if (count >= 0 && !array[count])
	{
		ft_free(array, count);
		return (NULL);
	}
	return (array);
}
