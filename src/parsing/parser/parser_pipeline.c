/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*get_next_command(t_token **token)
{
	if ((*token)->type == TOKEN_LPAREN)
		return (parse_parentheses(token));
	return (parse_command(token));
}

t_nodes	*parse_pipeline(t_token **token)
{
	t_nodes	*first_cmd;
	t_nodes	*current;
	t_nodes	*next_cmd;

	first_cmd = get_next_command(token);
	if (!first_cmd)
		return (NULL);
	current = first_cmd;
	while (*token && (*token)->type == TOKEN_PIPE)
	{
		*token = (*token)->next;
		next_cmd = get_next_command(token);
		if (!next_cmd)
		{
			free_node(first_cmd);
			return (NULL);
		}
		current->is_operator = 1;
		current->operator_type = TOKEN_PIPE;
		current->right = next_cmd;
		current = next_cmd;
	}
	return (first_cmd);
}
