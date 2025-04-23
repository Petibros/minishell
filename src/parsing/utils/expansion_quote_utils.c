/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quote_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 01:46:22 by npapashv          #+#    #+#             */
/*   Updated: 2025/04/12 01:46:24 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*handle_single_quote(char *str, int *i, char *result)
{
	int		start;
	char	*tmp;

	start = *i;  // Remember the position of the opening quote
	(*i)++;      // Skip the opening quote

	// Find the closing quote
	while (str[*i] && str[*i] != '\'')
		(*i)++;

	if (*i > start)
	{
		// Include everything from opening quote to closing quote
		tmp = ft_substr(str, start, *i - start + 1);
		if (!tmp)
			return (NULL);
		result = ft_strjoin_free(result, tmp);
		free(tmp);
	}

	if (str[*i] == '\'')
		(*i)++;  // Skip the closing quote

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
