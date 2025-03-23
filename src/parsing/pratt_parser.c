/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:11:38 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 03:11:38 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*handle_operator(t_nodes *left, t_token **token)
{
	t_nodes		*right;
	t_nodes		*op_node;
	t_token_type	op_type;
	int			precedence;

	op_type = (*token)->type;
	precedence = get_precedence(op_type);
	*token = (*token)->next;
	right = parse_expression(token, precedence);
	if (!right)
		return (NULL);
	op_node = create_op_node(left, right, op_type);
	if (!op_node)
	{
		free_node(right);
		return (NULL);
	}
	return (op_node);
}

t_nodes	*parse_atom(t_token **token)
{
	t_nodes	*node;
	t_token	*word_tokens;
	int		word_count;

	if (!*token)
		return (NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_parentheses(token));

	node = create_node();
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

t_nodes	*parse_expression(t_token **token, int min_precedence)
{
	t_nodes	*left;
	t_nodes	*new_left;

	left = parse_atom(token);
	if (!left)
		return (NULL);
	while (*token && get_precedence((*token)->type) > min_precedence)
	{
		new_left = handle_operator(left, token);
		if (!new_left)
		{
			free_node(left);
			return (NULL);
		}
		left = new_left;
	}
	return (left);
}

t_nodes	*pratt_parse(t_token *token)
{
	return (parse_expression(&token, 0));
}
