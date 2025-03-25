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

static t_nodes	*handle_operator(t_nodes *left, t_token **token, char **envp)
{
	t_nodes			*right;
	t_nodes			*op_node;
	t_token_type	op_type;
	int				precedence;

	op_type = (*token)->type;
	precedence = get_precedence(op_type);
	*token = (*token)->next;
	right = parse_expression(token, precedence, envp);
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

t_nodes	*parse_expression(t_token **token, int min_precedence, char **envp)
{
	t_nodes	*left;
	t_nodes	*new_left;

	left = parse_atom(token, envp);
	if (!left)
		return (NULL);
	while (*token && get_precedence((*token)->type) > min_precedence)
	{
		new_left = handle_operator(left, token, envp);
		if (!new_left)
		{
			free_node(left);
			return (NULL);
		}
		left = new_left;
	}
	return (left);
}

t_nodes	*pratt_parse(t_token *token, char **envp)
{
	return (parse_expression(&token, 0, envp));
}
