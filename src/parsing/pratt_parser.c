/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 02:28:40 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 02:28:40 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Function prototypes
static t_nodes	*parse_expression(t_token **token, int min_precedence);
static t_nodes	*parse_atom(t_token **token);

// Définition des précédences des opérateurs
static int	get_precedence(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (1);
	if (type == TOKEN_AND || type == TOKEN_OR)
		return (2);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || 
		type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (3);
	return (0);
}

static t_nodes	*create_node(void)
{
	t_nodes	*node;

	node = (t_nodes *)malloc(sizeof(t_nodes));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->argv = NULL;
	node->fd_in = STDIN_FILENO;
	node->fd_out = STDOUT_FILENO;
	node->append_out = 0;
	node->here_doc = 0;
	node->delimiter = NULL;
	node->next_operator = 0;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_nodes	*parse_atom(t_token **token)
{
	t_nodes	*node;
	int		argc;
	t_token	*start;

	if (!*token)
		return (NULL);
	if ((*token)->type == TOKEN_LPAREN)
	{
		*token = (*token)->next;
		node = parse_expression(token, 0);
		if (*token && (*token)->type == TOKEN_RPAREN)
			*token = (*token)->next;
		return (node);
	}
	node = create_node();
	if (!node)
		return (NULL);
	argc = 0;
	start = *token;
	while (*token && (*token)->type == TOKEN_WORD)
	{
		argc++;
		*token = (*token)->next;
	}
	if (argc > 0)
	{
		node->argv = (char **)malloc(sizeof(char *) * (argc + 1));
		if (!node->argv)
			return (NULL);
		argc = 0;
		while (start != *token)
		{
			node->argv[argc++] = ft_strdup(start->value);
			start = start->next;
		}
		node->argv[argc] = NULL;
		node->cmd = ft_strdup(node->argv[0]);
	}
	return (node);
}

static t_nodes	*parse_expression(t_token **token, int min_precedence)
{
	t_nodes		*left;
	t_nodes		*right;
	t_nodes		*op_node;
	t_token_type	op_type;
	int			precedence;

	left = parse_atom(token);
	if (!left)
		return (NULL);
	while (*token)
	{
		op_type = (*token)->type;
		precedence = get_precedence(op_type);
		if (precedence <= min_precedence)
			break ;
		*token = (*token)->next;
		right = parse_expression(token, precedence);
		if (!right)
		{
			free_node(left);
			return (NULL);
		}
		op_node = create_node();
		if (!op_node)
		{
			free_node(left);
			free_node(right);
			return (NULL);
		}
		op_node->next_operator = op_type;
		if (op_type == TOKEN_PIPE)
		{
			op_node->cmd = ft_strdup("|");
			op_node->left = left;
			op_node->right = right;
		}
		else if (op_type == TOKEN_AND)
		{
			op_node->cmd = ft_strdup("&&");
			op_node->left = left;
			op_node->right = right;
		}
		else if (op_type == TOKEN_OR)
		{
			op_node->cmd = ft_strdup("||");
			op_node->left = left;
			op_node->right = right;
		}
		left = op_node;
	}
	return (left);
}

t_nodes	*pratt_parse(t_token *token)
{
	return (parse_expression(&token, 0));
}
