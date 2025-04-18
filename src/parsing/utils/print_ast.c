/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:35:02 by npapash           #+#    #+#             */
/*   Updated: 2025/04/18 14:35:02 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	print_indentation(int depth)
{
	int	i;

	i = 0;
	while (i < depth)
	{
		ft_putstr_fd("  ", 2);
		i++;
	}
}

static void	print_redirection_list(t_redir *redir, char *op)
{
	while (redir)
	{
		ft_putchar_fd(' ', 2);
		ft_putstr_fd(op, 2);
		ft_putchar_fd(' ', 2);
		ft_putstr_fd(redir->filename, 2);
		redir = redir->next;
	}
}

static void	print_redirections(t_nodes *node)
{
	write(2, " file_in: ", 11);
	if (node->file_in)
		print_redirection_list(node->file_in, "<");
	write(2, " file_out: ", 12);
	if (node->file_out)
	{
		t_redir *out = node->file_out;
		while (out)
		{
			ft_putchar_fd(' ', 2);
			ft_putstr_fd(out->append ? ">>" : ">", 2);
			ft_putchar_fd(' ', 2);
			ft_putstr_fd(out->filename, 2);
			out = out->next;
		}
	}
	write(2, " heredoc: ", 11);
	if (node->heredoc)
		print_redirection_list(node->heredoc, "<<");
}

static void	print_command(t_nodes *node, int depth)
{
	int	i;

	print_indentation(depth);
	ft_putstr_fd("CMD: ", 2);
	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			ft_putstr_fd(node->argv[i], 2);
			if (node->argv[i + 1])
				ft_putchar_fd(' ', 2);
			i++;
		}
	}
	print_redirections(node);
	ft_putchar_fd('\n', 2);
}

static void	print_operator_type(t_token_type type)
{
	if (type == TOKEN_AND)
		ft_putstr_fd("AND", 2);
	else if (type == TOKEN_OR)
		ft_putstr_fd("OR", 2);
	else if (type == TOKEN_PIPE)
		ft_putstr_fd("PIPE", 2);
	else if (type == TOKEN_SUBSHELL)
		ft_putstr_fd("SUBSHELL", 2);
}

void	print_ast_recursive(t_nodes *node, int depth)
{
	if (!node)
		return ;
	print_indentation(depth);
	if (node->is_operator)
	{
		ft_putstr_fd("OP: ", 2);
		print_operator_type(node->operator_type);
		print_redirections(node);
		ft_putchar_fd('\n', 2);
		print_ast_recursive(node->left, depth + 1);
		print_ast_recursive(node->right, depth + 1);
	}
	else
		print_command(node, depth);
}

void	print_ast(t_nodes *root)
{
	ft_putstr_fd("\n=== AST ===\n", 2);
	print_ast_recursive(root, 0);
	ft_putstr_fd("==========\n", 2);
}
