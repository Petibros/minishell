/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:11:38 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 03:11:38 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*handle_operator(t_nodes *left, t_token **token)
{
	t_nodes		*right;
	t_nodes		*op_node;
	t_token_type	op_type;
	int			precedence;

	op_type = (*token)->type;
	precedence = get_precedence(op_type);
	*token = (*token)->next;
	right = parse_expression(token, precedence);
	if (!right)
		return (NULL);
	op_node = create_op_node(left, right, op_type);
	if (!op_node)
	{
		free_node(right);
		return (NULL);
	}
	return (op_node);
}

t_nodes	*parse_atom(t_token **token)
{
	t_nodes	*node;
	int		argc;

	if (!*token)
		return (NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_parentheses(token));
	argc = count_word_tokens(token);
	node = create_cmd_node(token, argc);
	if (!node)
		return (NULL);
	advance_token(token, argc);
	if (*token && ((*token)->type == TOKEN_REDIR_IN ||
		(*token)->type == TOKEN_REDIR_OUT ||
		(*token)->type == TOKEN_APPEND ||
		(*token)->type == TOKEN_HEREDOC))
	{
		if (!handle_redirections(node, token))
		{
			free_node(node);
			return (NULL);
		}
	}
	return (node);
}

t_nodes	*parse_expression(t_token **token, int min_precedence)
{
	t_nodes	*left;
	t_nodes	*new_left;

	left = parse_atom(token);
	if (!left)
		return (NULL);
	while (*token && get_precedence((*token)->type) > min_precedence)
	{
		new_left = handle_operator(left, token);
		if (!new_left)
		{
			free_node(left);
			return (NULL);
		}
		left = new_left;
	}
	return (left);
}

t_nodes	*pratt_parse(t_token *token)
{
	return (parse_expression(&token, 0));
}
