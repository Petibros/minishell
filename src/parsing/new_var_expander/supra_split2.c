/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supra_split2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:57:01 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 11:57:03 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_sep(char c, char *seps);

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_in_quotes(const char *s, int pos)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (i < pos)
	{
		if (s[i] == '\\' && i + 1 < pos && is_quote(s[i + 1]))
			i += 2;
		else if (s[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
		}
		else if (s[i] == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
		}
		else
			i++;
	}
	return (in_single_quote || in_double_quote);
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

int	fill_array(char const *s, char *seps, char **array, int count)
{
	int	i;
	int	j;
	int	start;

	start = 0;
	j = 0;
	i = 0;
	while (j < count)
	{
		if (is_sep(s[i], seps) && !is_in_quotes(s, i) && is_sep(s[i - 1], seps))
			start = -1;
		else if ((is_sep(s[i], seps) && !is_in_quotes(s, i)) || s[i] == '\0')
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

int	count_strings(char const *s, char *seps)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (is_sep(s[i], seps) && !is_in_quotes(s, i)
			&& (i == 0 || !is_sep(s[i - 1], seps) || s[i + 1] == '\0'))
			count++;
		else if (s[i + 1] == '\0' && !is_sep(s[i], seps))
			count++;
		i++;
	}
	return (count);
}
