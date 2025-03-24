/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_or.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*get_next_and_or_cmd(t_token **token)
{
	if ((*token)->type == TOKEN_LPAREN)
		return (parse_parentheses(token));
	return (parse_pipeline(token));
}

static int	process_next_cmd(t_nodes *current, t_token **token,
			t_token_type op_type)
{
	t_nodes	*next_cmd;

	next_cmd = get_next_and_or_cmd(token);
	if (!next_cmd)
		return (0);
	current->is_operator = 1;
	current->operator_type = op_type;
	current->right = next_cmd;
	return (1);
}

t_nodes	*parse_and_or(t_token **token)
{
	t_nodes			*first_cmd;
	t_nodes			*current;
	t_token_type	op_type;

	first_cmd = parse_pipeline(token);
	if (!first_cmd)
		return (NULL);
	current = first_cmd;
	while (*token && ((*token)->type == TOKEN_AND
			|| (*token)->type == TOKEN_OR))
	{
		op_type = (*token)->type;
		*token = (*token)->next;
		if (!process_next_cmd(current, token, op_type))
		{
			free_node(first_cmd);
			return (NULL);
		}
		current = current->right;
	}
	return (first_cmd);
}
