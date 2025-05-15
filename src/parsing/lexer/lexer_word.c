/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:42:08 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:59:56 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "lexer_word2.h"


static int	get_word_len(char *input)
{
	int		len;
	char	quote;
	int		result;

	len = 0;
	quote = 0;
	while (input[len])
	{
		result = process_word_char(input, &len, &quote);
		if (result == 1)
			break ;
		if (result == 2)
			return (2);
	}
	return (len);
}

char	*get_word(char **input)
{
	int		len;
	char	*word;
	int		i;

	len = get_word_len(*input);
	if (len <= 0)
		return (NULL);
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
