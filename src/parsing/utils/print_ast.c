/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 08:27:15 by npapash           #+#    #+#             */
/*   Updated: 2025/03/31 08:27:15 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "print_ast.h"

static void	print_command_node(t_nodes *node)
{
	int	i;

	if (!node || !node->argv || !node->argv[0])
		return ;
	printf("Command: %s\n", node->argv[0]);
	if (node->argv[1])
	{
		printf("Arguments:\n");
		i = 1;
		while (node->argv[i])
		{
			printf("  [%d]: %s\n", i - 1, node->argv[i]);
			i++;
		}
	}
}

void	print_ast_node(t_nodes *node)
{
	if (!node)
		return ;
	if (!node->is_operator)
		print_command_node(node);
	else if (node->operator_type == TOKEN_PIPE)
	{
		printf("Pipe:\n");
		printf("Left branch:\n");
		print_ast_node(node->left);
		printf("Right branch:\n");
		print_ast_node(node->right);
	}
}
