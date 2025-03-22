/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include "expander.h"

static char	*handle_dollar_in_dquote(char *str, int *i, char *result,
		int exit_status)
{
	char	*tmp;

	tmp = expand_env_var(str, i, exit_status);
	if (tmp)
	{
		result = ft_strjoin_free(result, tmp);
		free(tmp);
	}
	return (result);
}

static char	*process_double_quote_content(char *str, int *i, char *result,
		int exit_status)
{
	int		start;

	start = *i;
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
		{
			if (start != *i)
				result = append_substring(result, str, start, *i);
			result = handle_dollar_in_dquote(str, i, result, exit_status);
			start = *i;
		}
		else
			(*i)++;
	}
	if (str[*i] == '"' && start != *i)
	{
		result = append_substring(result, str, start, *i);
		(*i)++;
	}
	return (result);
}

char	*handle_double_quote_char(char *str, int *i, char *result,
		int exit_status)
{
	(*i)++;
	return (process_double_quote_content(str, i, result, exit_status));
}
