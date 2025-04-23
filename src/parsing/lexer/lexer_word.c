/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:42:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:42:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	is_dollar_operator(char *input, int pos)
{
	return (input[pos] == '$' && input[pos + 1] && 
		(input[pos + 1] == '>' || input[pos + 1] == '<'));
}

static int	get_word_len(char *input)
{
	int		len;
	char	quote;

	len = 0;
	quote = 0;
	while (input[len])
	{
		if (!quote && (input[len] == '\'' || input[len] == '\"'))
			quote = input[len];
		else if (quote && input[len] == quote)
		{
			if (quote == '\'')  // If in single quotes, treat everything literally
				len++;
			quote = 0;
			if (!input[len])  // If we're at the end after closing quote
				break;
		}
		if (!quote)
		{
			if (is_whitespace(input[len]) || 
				(is_operator_char(input[len]) && !is_dollar_operator(input, len)))
				break;
			if (is_dollar_operator(input, len))
			{
				if (len == 0)
					len = 2;  // Return $> as a single token
				break;
			}
		}
		len++;
	}
	return (len);
}

char	*get_word(char **input)
{
	int		len;
	char	*word;
	int		i;

	len = get_word_len(*input);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = (*input)[i];
		i++;
	}
	word[i] = '\0';
	*input += len;
	return (word);
}
