/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_nodes	*create_parser_node(void)
{
	t_nodes	*node;

	node = (t_nodes *)malloc(sizeof(t_nodes));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->argv = NULL;
	node->file_in = NULL;
	node->file_out = NULL;
	node->heredoc = NULL;
	node->is_operator = 0;
	node->operator_type = TOKEN_EOF;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
