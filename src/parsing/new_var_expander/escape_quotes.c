/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 04:00:00 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/16 04:00:00 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_quotes(const char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			count++;
		i++;
	}
	return (count);
}

char	*escape_quotes_in_var_value(const char *str)
{
	char	*result;
	int		quotes_count;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	quotes_count = count_quotes(str);
	if (quotes_count == 0)
		return (ft_strdup(str));
	result = malloc(ft_strlen(str) + quotes_count + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			result[j++] = '\\';
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
