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

	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (*i > start)
	{
		tmp = ft_substr(str, start, *i - start + 1);
		if (!tmp)
			return (NULL);
		result = ft_strjoin_free(result, tmp);
		free(tmp);
	}
	if (str[*i] == '\'')
		(*i)++;
	return (result);
}