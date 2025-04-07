/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*handle_single_quote(char *str, int *i, char *result)
{
	char	*tmp;
	int		start;

	(*i)++;  /* Skip the opening single quote */
	
	/* Remember the start position after the opening quote */
	start = *i;
	
	/* Find the closing quote */
	while (str[*i] && str[*i] != '\'')
		(*i)++;
		
	/* Extract the content between quotes exactly as is */
	if (*i > start)
	{
		tmp = ft_substr(str, start, *i - start);
		if (!tmp)
			return (NULL);
		result = ft_strjoin_free(result, tmp);
		free(tmp);
	}
	
	if (str[*i] == '\'')
		(*i)++;  /* Skip the closing quote */
		
	return (result);
}

int	copy_expanded_entries(char **new_argv, int j, char *arg)
{
	int	count;
	int	old_j;

	old_j = j;
	count = expand_wildcard_entry(arg, new_argv, &j);
	if (count > 0)
		return (j);
	return (old_j + 1);
}
