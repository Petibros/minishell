/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:34:38 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:41:48 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	process_single_quote(char *str, char *result, int *i, int *j)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		result[(*j)++] = str[(*i)++];
	}
	if (str[*i] == '\'')
		(*i)++;
}

static void	process_double_quote(char *str, char *result, int *i, int *j)
{
	(*i)++;
	while (str[*i] && str[*i] != '\"')
	{
		result[(*j)++] = str[*i];
		(*i)++;
	}
	if (str[*i] == '\"')
		(*i)++;
}

static void	process_regular_char(char *str, char *result, int *i, int *j)
{
	result[(*j)] = str[*i];
	(*j)++;
	(*i)++;
}

void	process_quotes(char *str, char *result, int *j)
{
	int		i;

	i = 0;
	*j = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			process_single_quote(str, result, &i, j);
		else if (str[i] == '\"')
			process_double_quote(str, result, &i, j);
		else
			process_regular_char(str, result, &i, j);
	}
}
