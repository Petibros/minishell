/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:11:38 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 03:11:38 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_nodes	*create_node(void)
{
	t_nodes	*node;

	node = malloc(sizeof(t_nodes));
	if (!node)
		return (NULL);
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
