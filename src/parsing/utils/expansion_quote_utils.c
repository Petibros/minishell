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
	int		is_ansi_c;

	is_ansi_c = (*i > 0 && str[*i - 1] == '$');
	if (!is_ansi_c)
	{
		tmp = ft_strjoin_free(result, "'");
		if (!tmp)
			return (NULL);
		result = tmp;
	}
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		result = handle_regular_char(str, i, result);
	if (str[*i] == '\'')
	{
		if (!is_ansi_c)
		{
			tmp = ft_strjoin_free(result, "'");
			if (!tmp)
				return (NULL);
			result = tmp;
		}
		(*i)++;
	}
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
