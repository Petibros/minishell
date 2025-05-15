/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_node_op.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 07:31:35 by npapash           #+#    #+#             */
/*   Updated: 2025/03/29 07:31:35 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	set_op_node_basics(t_nodes *node, t_nodes *left,
	t_nodes *right, t_token_type op_type)
{
	node->is_operator = 1;
	node->operator_type = op_type;
	node->left = left;
	node->right = right;
	node->argv = malloc(sizeof(char *) * 2);
	if (!node->argv)
		return (0);
	return (1);
}

static int	set_op_node_argv(t_nodes *node, t_token_type op_type)
{
	if (op_type == TOKEN_PIPE)
		node->argv[0] = ft_strdup("|");
	else if (op_type == TOKEN_AND)
		node->argv[0] = ft_strdup("&&");
	else if (op_type == TOKEN_OR)
		node->argv[0] = ft_strdup("||");
	if (!node->argv[0])
		return (0);
	node->argv[1] = NULL;
	return (1);
}

t_nodes	*create_op_node(t_nodes *left, t_nodes *right, t_token_type op_type)
{
	t_nodes	*node;

	node = create_node();
	if (!node)
		return (NULL);
	if (!set_op_node_basics(node, left, right, op_type)
		|| !set_op_node_argv(node, op_type))
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}

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