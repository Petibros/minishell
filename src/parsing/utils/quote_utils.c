/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
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

char	*remove_quotes(char *str)
{
	char	*result;
	char	quote;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (count_chars_without_quotes(str) + 1));
	if (!result)
		return (NULL);
	quote = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!quote && is_quote(str[i]))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			result[j++] = str[i];
		i++;
	}
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

void	handle_quotes_in_node(t_nodes *node)
{
	char	*tmp;
	int		i;

	if (!node || !node->argv)
		return ;
	i = 0;
	while (node->argv[i])
	{
		tmp = remove_quotes(node->argv[i]);
		if (tmp)
		{
			free(node->argv[i]);
			node->argv[i] = tmp;
		}
		i++;
	}
	if (node->cmd)
	{
		tmp = remove_quotes(node->cmd);
		if (tmp)
		{
			free(node->cmd);
			node->cmd = tmp;
		}
	}
	if (node->delimiter)
	{
		tmp = remove_quotes(node->delimiter);
		if (tmp)
		{
			free(node->delimiter);
			node->delimiter = tmp;
		}
	}
}
