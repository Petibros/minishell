/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:57:31 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 12:03:11 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "lexer_word2.h"

static int	handle_quote(char quote, int *len)
{
	if (quote == '\'')
		(*len)++;
	return (0);
}

static int	check_special_chars(char *input, int len)
{
	if (is_whitespace(input[len])
		|| is_operator_char(input[len]))
		return (1);
	return (0);
}

int	process_word_char(char *input, int *len, char *quote)
{
	int	special;

	if (!*quote && (input[*len] == '\'' || input[*len] == '\"'))
	{
		*quote = input[*len];
		(*len)++;
		return (0);
	}
	if (*quote && input[*len] == *quote)
	{
		*quote = handle_quote(*quote, len);
		if (!input[*len])
			return (1);
	}
	if (!*quote)
	{
		special = check_special_chars(input, *len);
		if (special == 1)
			return (1);
		if (special == 2)
			return (2);
	}
	(*len)++;
	return (0);
}
