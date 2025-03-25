/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parentheses.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_nodes	*parse_parentheses(t_token **token, char **envp)
{
	t_nodes	*node;
	t_nodes	*inner_cmd;

	if (!*token || (*token)->type != TOKEN_LPAREN)
		return (NULL);
	*token = (*token)->next;
	inner_cmd = parse_and_or(token, envp);
	if (!inner_cmd)
		return (NULL);
	if (!*token || (*token)->type != TOKEN_RPAREN)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	*token = (*token)->next;
	node = create_parser_node();
	if (!node)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	node->is_operator = 1;
	node->operator_type = TOKEN_LPAREN;
	node->right = inner_cmd;
	return (node);
}
