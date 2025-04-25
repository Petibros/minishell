/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:09:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/18 20:09:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	count_chars_without_quotes(char *str)
{
	int		count;
	char	quote;
	int		i;

	count = 0;
	quote = 0;
	i = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			count++;
		i++;
	}
	return (count);
}

static int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static void	process_quotes(char *str, char *result, int *j)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	*j = 0;
	while (str[i])
	{
		if (!quote && is_quote(str[i]))
		{
			quote = str[i];
			if (quote == '\'')
			{
				i++;
				while (str[i] && str[i] != '\'')
				{
					result[(*j)++] = str[i++];
				}
				if (str[i] == '\'')
					i++;
				quote = 0;
				continue ;
			}
		}
		else if (quote && str[i] == quote)
			quote = 0;
		else
		{
			result[(*j)] = str[i];
			(*j)++;
		}
		i++;
	}
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		j;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (count_chars_without_quotes(str) + 1));
	if (!result)
		return (NULL);
	process_quotes(str, result, &j);
	result[j] = '\0';
	return (result);
}

int	check_quotes(char *str)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (str[i])
	{
		if (!quote && is_quote(str[i]))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		i++;
	}
	return (!quote);
}
