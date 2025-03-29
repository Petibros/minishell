/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_node_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 07:31:35 by npapash           #+#    #+#             */
/*   Updated: 2025/03/29 07:31:35 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	alloc_cmd_argv(t_nodes *node, int argc)
{
	node->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!node->argv)
		return (0);
	return (1);
}

static int	fill_cmd_argv(t_nodes *node, t_token **token, int argc)
{
	int	i;

	i = 0;
	while (i < argc)
	{
		node->argv[i] = ft_strdup((*token)->value);
		if (!node->argv[i])
			return (0);
		*token = (*token)->next;
		i++;
	}
	node->argv[argc] = NULL;
	return (1);
}

t_nodes	*create_cmd_node(t_token **token, int argc)
{
	t_nodes	*node;

	node = create_node();
	if (!node || argc <= 0)
		return (NULL);
	if (!alloc_cmd_argv(node, argc)
		|| !fill_cmd_argv(node, token, argc))
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}
