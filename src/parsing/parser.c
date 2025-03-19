/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:09:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/18 20:09:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Function prototypes
static t_nodes	*create_node(void);
static t_nodes	*parse_command(t_token **token);
static t_nodes	*parse_pipeline(t_token **token);
static t_nodes	*parse_and_or(t_token **token);
static t_nodes	*parse_parentheses(t_token **token);

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

static t_nodes	*parse_command(t_token **token)
{
	t_nodes	*node;
	int		argc;
	t_token	*start;

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

static t_nodes	*parse_pipeline(t_token **token)
{
	t_nodes	*first_cmd;
	t_nodes	*current;
	t_nodes	*next_cmd;

	// Check if first token is a parenthesis
	if (*token && (*token)->type == TOKEN_LPAREN)
		first_cmd = parse_parentheses(token);
	else
		first_cmd = parse_command(token);

	if (!first_cmd)
		return (NULL);

	current = first_cmd;

	// Handle pipe operators
	while (*token && (*token)->type == TOKEN_PIPE)
	{
		*token = (*token)->next;

		// Check if next token is a parenthesis
		if (*token && (*token)->type == TOKEN_LPAREN)
		{
			// Parse parenthesized expression
			next_cmd = parse_parentheses(token);
		}
		else
		{
			// Parse simple command
			next_cmd = parse_command(token);
		}

		if (!next_cmd)
		{
			free_node(first_cmd);
			return (NULL);
		}

		// Set up pipe structure
		current->next_operator = TOKEN_PIPE;
		current->left = next_cmd;

		// Move to next command
		current = next_cmd;
	}

	return (first_cmd);
}

static t_nodes	*parse_and_or(t_token **token)
{
	t_nodes	*first_cmd;
	t_nodes	*next_cmd;
	t_nodes	*current;
	t_nodes	*last_in_pipeline;

	// Parse the first command (might be a pipeline)
	first_cmd = parse_pipeline(token);
	if (!first_cmd)
		return (NULL);

	current = first_cmd;

	// Handle AND/OR operators
	while (*token && ((*token)->type == TOKEN_AND || (*token)->type == TOKEN_OR))
	{
		t_token_type op_type = (*token)->type;
		*token = (*token)->next;

		// Parse next command (might be a pipeline)
		if ((*token) && (*token)->type == TOKEN_LPAREN)
			next_cmd = parse_parentheses(token);
		else
			next_cmd = parse_pipeline(token);

		if (!next_cmd)
			return (NULL);

		// Find the last command in the current pipeline
		last_in_pipeline = current;
		while (last_in_pipeline->left && last_in_pipeline->next_operator == TOKEN_PIPE)
			last_in_pipeline = last_in_pipeline->left;

		// Set operator and link next command
		last_in_pipeline->next_operator = op_type;

		// For AND, add to left branch and move to next command
		if (op_type == TOKEN_AND)
		{
			last_in_pipeline->left = next_cmd;
			current = next_cmd;
		}
		else // For OR, add to right branch and stay on current
		{
			// Find the end of the OR chain in the right branch
			if (!last_in_pipeline->right)
				last_in_pipeline->right = next_cmd;
			else
			{
				t_nodes *or_end = last_in_pipeline->right;
				while (or_end->right)
					or_end = or_end->right;
				or_end->next_operator = TOKEN_OR;
				or_end->right = next_cmd;
			}
		}
	}

	return (first_cmd);
}

static t_nodes	*parse_parentheses(t_token **token)
{
	t_nodes	*cmd;
	t_nodes	*result;

	if (!*token || (*token)->type != TOKEN_LPAREN)
		return (NULL);

	// Skip opening parenthesis
	*token = (*token)->next;

	// Parse expression inside parentheses using parse_and_or
	cmd = parse_and_or(token);
	if (!cmd || !*token)
		return (NULL);

	// Expect closing parenthesis
	if ((*token)->type != TOKEN_RPAREN)
	{
		free_node(cmd);
		return (NULL);
	}
	*token = (*token)->next;

	// If there's an operator after the parentheses
	if (*token && ((*token)->type == TOKEN_AND || (*token)->type == TOKEN_OR || (*token)->type == TOKEN_PIPE))
	{
		t_token_type op_type = (*token)->type;
		*token = (*token)->next;

		// Parse next command
		if ((*token) && (*token)->type == TOKEN_LPAREN)
			result = parse_parentheses(token);
		else if (op_type == TOKEN_PIPE)
			result = parse_pipeline(token);
		else
			result = parse_and_or(token);

		if (!result)
		{
			free_node(cmd);
			return (NULL);
		}

		// Add the command to the appropriate branch
		cmd->next_operator = op_type;
		if (op_type == TOKEN_OR)
			cmd->right = result;
		else // TOKEN_AND or TOKEN_PIPE
			cmd->left = result;
	}

	return (cmd);
}

t_nodes	*parse(t_token *token)
{
	// Start with parse_and_or which handles all cases
	return (parse_and_or(&token));
}
