/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:00:55 by npapash           #+#    #+#             */
/*   Updated: 2025/04/28 12:00:55 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	print_indent(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
}

static void	print_redirs(t_redir *redir, const char *type, int depth)
{
	while (redir)
	{
		print_indent(depth + 1);
		printf("%s: %s", type, redir->filename);
		if (redir->append)
			printf(" (append)");
		if (redir->quoted)
			printf(" (quoted)");
		printf("\n");
		redir = redir->next;
	}
}

static void	print_command(t_nodes *node, int depth)
{
	int	i;

	print_indent(depth);
	printf("Command:");
	i = 0;
	while (node->argv && node->argv[i])
	{
		printf(" %s", node->argv[i]);
		i++;
	}
	printf("\n");
	if (node->file_in)
	{
		print_indent(depth);
		printf("Input redirections:\n");
		print_redirs(node->file_in, "Input", depth);
	}
	if (node->file_out)
	{
		print_indent(depth);
		printf("Output redirections:\n");
		print_redirs(node->file_out, "Output", depth);
	}
	if (node->heredoc)
	{
		print_indent(depth);
		printf("Heredocs:\n");
		print_redirs(node->heredoc, "Heredoc", depth);
	}
}

void	print_ast_with_redirs(t_nodes *node, int depth)
{
	if (!node)
		return ;
	if (node->is_operator)
	{
		print_indent(depth);
		printf("Operator: ");
		if (node->operator_type == TOKEN_PIPE)
			printf("PIPE");
		else if (node->operator_type == TOKEN_AND)
			printf("AND");
		else if (node->operator_type == TOKEN_OR)
			printf("OR");
		printf("\n");
		print_ast_with_redirs(node->left, depth + 1);
		print_ast_with_redirs(node->right, depth + 1);
	}
	else
		print_command(node, depth);
}
