/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_node_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 07:22:14 by npapash           #+#    #+#             */
/*   Updated: 2025/03/29 07:22:14 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*get_operator_string(t_token_type op_type)
{
	if (op_type == TOKEN_PIPE)
		return (ft_strdup("|"));
	else if (op_type == TOKEN_AND)
		return (ft_strdup("&&"));
	else if (op_type == TOKEN_OR)
		return (ft_strdup("||"));
	return (NULL);
}

t_nodes	*init_op_node(t_nodes *node, t_nodes *left,
	t_nodes *right, t_token_type op_type)
{
	node->is_operator = 1;
	node->operator_type = op_type;
	node->left = left;
	node->right = right;
	node->argv = (char **)malloc(sizeof(char *) * 2);
	if (!node->argv)
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}

t_nodes	*init_cmd_argv(t_nodes *node, t_token *start, int argc)
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
