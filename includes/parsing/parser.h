/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:20:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:20:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "parsing.h"

/* Parser functions */
t_nodes		*parse(t_token *token);
t_nodes		*pratt_parse(t_token *token);
int			parse_line(t_vars *vars);

/* Pratt parser functions */
t_nodes		*parse_expression(t_token **token, int min_precedence);
t_nodes		*parse_atom(t_token **token);
t_nodes		*handle_parentheses(t_token **token);
int			get_precedence(t_token_type type);
int			count_word_tokens(t_token **token);
void		advance_token(t_token **token, int count);

#endif
