/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:20:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:20:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "types.h"

/* Utility functions */
int		is_operator_char(char c);
int		is_whitespace(char c);
t_token		*create_token(t_token_type type, char *value);

/* Word handling */
char		*get_word(char **input);

/* Operator handling */
t_token		*handle_lexer_operator(char **input);

/* Main lexer functions */
t_token		*get_next_token(char **input);
t_token		*lexer(char *input);

#endif
