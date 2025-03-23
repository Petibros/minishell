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

# include "parsing.h"

/* Lexer functions */
t_token		*lexer(char *input);
t_token		*get_next_token(char **input);

#endif
