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

static int	get_word_len(char *input)
{
	int		len;
	char	quote;

	len = 0;
	quote = 0;
	while (input[len] && (!is_operator_char(input[len]) || quote)
		&& (!is_whitespace(input[len]) || quote))
	{
		if (input[len] == '\'' || input[len] == '\"')
		{
			if (!quote)
				quote = input[len];
			else if (quote == input[len])
				quote = 0;
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
	word = (char *)malloc(sizeof(char) * (len + 1));
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
