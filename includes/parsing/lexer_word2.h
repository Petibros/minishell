/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word2.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:57:31 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 12:03:11 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_WORD2_H
# define LEXER_WORD2_H

# include "parsing.h"

int	is_dollar_redir_operator(char *input, int pos);
int	process_word_char(char *input, int *len, char *quote);

#endif
