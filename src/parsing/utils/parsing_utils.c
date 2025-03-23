/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:09:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/18 20:09:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdio.h>

static const char *g_token_types[] = {
	"WORD",
	"PIPE",
	"AND",
	"OR",
	"REDIR_IN",
	"REDIR_OUT",
	"APPEND",
	"HEREDOC",
	"LPAREN",
	"RPAREN",
	"EOF"
};

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_token(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = next;
	}
}

void	free_node(t_nodes *node)
{
	if (!node)
		return ;
	if (node->cmd)
		free(node->cmd);
	if (node->argv)
		free_array(node->argv);
	if (node->delimiter)
		free(node->delimiter);
	if (node->file_in)
		free(node->file_in);
	if (node->file_out)
		free(node->file_out);
	free_node(node->left);
	free_node(node->right);
	free(node);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	result[len1 + len2] = '\0';
	free(s1);
	return (result);
}

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
}

void	print_token_list(t_token *token)
{
	printf("\n=== Token List ===\n");
	while (token)
	{
		printf("Type: %-10s | Value: '%s'\n",
			g_token_types[token->type],
			token->value ? token->value : "NULL");
		token = token->next;
	}
	printf("================\n");
}

static void	print_node_recursive(t_nodes *node, int depth)
{
	int	i;

	if (!node)
		return ;

	for (i = 0; i < depth; i++)
		printf("  ");

	// Print command, op_type, and arguments
	printf("Command: %s\n", node->cmd ? node->cmd : "NULL");
	printf("is_operator: %d\n", node->is_operator);
	if (node->is_operator)
		printf("operator_type: %s\n", g_token_types[node->operator_type]);
	if (node->argv)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Args: ");
		for (i = 0; node->argv[i]; i++)
			printf("%s ", node->argv[i]);
		printf("\n");
	}

	// Print redirections
	if (node->file_in || node->file_out ||
		node->here_doc || node->delimiter)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Redirections:\n");
		if (node->file_in)
			printf("    Input File: %s\n", node->file_in);
		if (node->file_out)
			printf("    Output File: %s (Append: %s)\n",
				node->file_out, node->append_out ? "yes" : "no");
		if (node->here_doc)
			printf("    Heredoc with delimiter: %s\n", node->delimiter);
	}

	// Print branches
	if (node->left || node->right)
	{
		if (node->left)
		{
			for (i = 0; i < depth; i++)
				printf("  ");
			printf("Left branch:\n");
			print_node_recursive(node->left, depth + 1);
		}
		if (node->right)
		{
			for (i = 0; i < depth; i++)
				printf("  ");
			printf("Right branch:\n");
			print_node_recursive(node->right, depth + 1);
		}
	}
	else if (node->is_operator)
	{
		// Show NULL branches when there's an operator
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Left branch: NULL\n");
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Right branch: NULL\n");
	}
}

void	print_ast(t_nodes *root)
{
	printf("\n=== Abstract Syntax Tree ===\n");
	print_node_recursive(root, 0);
	printf("==========================\n");
}
