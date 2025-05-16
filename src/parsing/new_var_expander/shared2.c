/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:57:26 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 11:58:07 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c);

static int	is_escaped_quote(char *str, int i)
{
	return (str[i] == '\\' && (str[i + 1] == '"' || str[i + 1] == '\'')
		&& str[i + 1] != '\0');
}

void	new_free_arr(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	handle_escaped_quote(char *str, int *i, int *j)
{
	str[*j] = str[*i + 1];
	(*j)++;
	*i += 2;
	return (1);
}

static int	handle_content_within_quotes(char *str, int *i, int *j, char quote)
{
	while (str[*i] && (str[*i] != quote
			|| (str[*i] == quote && *i > 0 && str[*i - 1] == '\\')))
	{
		if (str[*i] == '\\' && str[*i + 1] == quote && str[*i + 1] != '\0')
		{
			str[*j] = str[*i + 1];
			(*j)++;
			*i += 2;
		}
		else
		{
			str[*j] = str[*i];
			(*j)++;
			(*i)++;
		}
	}
	return (1);
}

void	new_remove_quotes(char *str)
{
	int		i;
	int		j;
	char	quote;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_escaped_quote(str, i))
			handle_escaped_quote(str, &i, &j);
		else if (is_quote(str[i]) && (i == 0 || str[i - 1] != '\\'))
		{
			quote = str[i++];
			handle_content_within_quotes(str, &i, &j, quote);
			if (str[i] == quote)
				i++;
		}
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}
