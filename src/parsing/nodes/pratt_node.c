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

	node = (t_nodes *)malloc(sizeof(t_nodes));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->argv = NULL;
	node->file_in = NULL;
	node->file_out = NULL;
	node->append_out = 0;
	node->here_doc = 0;
	node->delimiter = NULL;
	node->is_operator = 0;
	node->operator_type = TOKEN_EOF;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_nodes	*create_op_node(t_nodes *left, t_nodes *right, t_token_type op_type)
{
	t_nodes	*node;

	node = create_node();
	if (!node)
		return (NULL);
	node->is_operator = 1;
	node->operator_type = op_type;
	node->left = left;
	node->right = right;
	if (op_type == TOKEN_PIPE)
		node->cmd = ft_strdup("|");
	else if (op_type == TOKEN_AND)
		node->cmd = ft_strdup("&&");
	else if (op_type == TOKEN_OR)
		node->cmd = ft_strdup("||");
	if (!node->cmd)
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}

static t_nodes	*init_cmd_argv(t_nodes *node, t_token *start, int argc)
{
	int	i;

	node->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!node->argv)
	{
		free_node(node);
		return (NULL);
	}
	i = 0;
	while (i < argc)
	{
		node->argv[i] = ft_strdup(start->value);
		if (!node->argv[i])
		{
			free_node(node);
			return (NULL);
		}
		start = start->next;
		i++;
	}
	node->argv[argc] = NULL;
	return (node);
}

t_nodes	*create_cmd_node(t_token **token, int argc)
{
	t_nodes	*node;

	node = create_node();
	if (!node || argc <= 0)
		return (NULL);
	node = init_cmd_argv(node, *token, argc);
	if (!node)
		return (NULL);
	node->cmd = ft_strdup(node->argv[0]);
	return (node);
}
