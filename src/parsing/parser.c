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
static t_nodes	*create_parser_node(void);
static t_nodes	*parse_command(t_token **token);
static t_nodes	*parse_pipeline(t_token **token);
static t_nodes	*parse_and_or(t_token **token);
static t_nodes	*parse_parentheses(t_token **token);

static t_nodes	*create_parser_node(void)
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

static t_nodes	*parse_command(t_token **token)
{
	t_nodes	*node;
	t_token	*word_tokens;
	int		word_count;

	node = create_parser_node();
	if (!node)
		return (NULL);

	// Save all word tokens while handling redirections
	word_tokens = NULL;
	word_count = 0;

	// Process all tokens, collecting words and handling redirections
	while (*token && ((*token)->type == TOKEN_WORD ||
		(*token)->type == TOKEN_REDIR_IN || (*token)->type == TOKEN_REDIR_OUT ||
		(*token)->type == TOKEN_APPEND || (*token)->type == TOKEN_HEREDOC))
	{
		if ((*token)->type == TOKEN_WORD)
		{
			// Save word token
			t_token *new_word = malloc(sizeof(t_token));
			if (!new_word)
			{
				free_node(node);
				return (NULL);
			}
			new_word->type = TOKEN_WORD;
			new_word->value = ft_strdup((*token)->value);
			new_word->next = NULL;
			if (!new_word->value)
			{
				free(new_word);
				free_node(node);
				return (NULL);
			}
			if (!word_tokens)
				word_tokens = new_word;
			else
			{
				new_word->next = word_tokens;
				word_tokens = new_word;
			}
			word_count++;
			*token = (*token)->next;
		}
		else if (!handle_redirections(node, token))
		{
			free_node(node);
			while (word_tokens)
			{
				t_token *next = word_tokens->next;
				free(word_tokens->value);
				free(word_tokens);
				word_tokens = next;
			}
			return (NULL);
		}
	}

	// Process word tokens if any were found
	if (word_count > 0)
	{
		node->argv = (char **)malloc(sizeof(char *) * (word_count + 1));
		if (!node->argv)
		{
			free_node(node);
			while (word_tokens)
			{
				t_token *next = word_tokens->next;
				free(word_tokens->value);
				free(word_tokens);
				word_tokens = next;
			}
			return (NULL);
		}

		// Copy words in reverse order (since we stored them in reverse)
		int i = word_count - 1;
		while (word_tokens)
		{
			node->argv[i--] = ft_strdup(word_tokens->value);
			t_token *next = word_tokens->next;
			free(word_tokens->value);
			free(word_tokens);
			word_tokens = next;
		}
		node->argv[word_count] = NULL;
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
		current->is_operator = 1;
		current->operator_type = TOKEN_PIPE;
		current->right = next_cmd;

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
		{
			free_node(first_cmd);
			return (NULL);
		}

		// Create a new node for the operator
		t_nodes *op_node = create_node();
		if (!op_node)
		{
			free_node(first_cmd);
			free_node(next_cmd);
			return (NULL);
		}

		// Set up operator node
		op_node->is_operator = 1;
		op_node->operator_type = op_type;
		op_node->cmd = ft_strdup(op_type == TOKEN_AND ? "&&" : "||");

		// For AND, success path (left) is next command
		// For OR, failure path (right) is next command
		if (op_type == TOKEN_AND)
		{
			op_node->right = current;
			op_node->left = next_cmd;
		}
		else // TOKEN_OR
		{
			op_node->right = next_cmd;
			op_node->left = current;
		}

		// Update current to the operator node
		current = op_node;
	}

	return (current);
}

static t_nodes	*parse_parentheses(t_token **token)
{
	t_nodes	*cmd;

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

	return (cmd);
}

t_nodes	*parse(t_token *token)
{
	// Start with parse_and_or which handles all cases
	return (parse_and_or(&token));
}
