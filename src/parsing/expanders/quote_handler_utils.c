/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:46:19 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:46:19 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include "expander.h"

char	*append_substring(char *result, char *str, int start, int end)
{
	char	*tmp;

	tmp = ft_substr(str, start, end - start);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	return (result);
}

char	*handle_regular_char(char *str, int *i, char *result)
{
	char	*tmp;

	tmp = append_substring(result, str, *i, *i + 1);
	(*i)++;
	return (tmp);
}

char	*handle_quote_char(char *str, int *i, char *result)
{
	char	quote;
	int		start;

	quote = str[(*i)++];
	start = *i;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] == quote)
	{
		result = append_substring(result, str, start, *i);
		(*i)++;
	}
	return (result);
}
