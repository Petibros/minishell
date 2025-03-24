/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parser_atom.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:39:23 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:39:23 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	process_word_token(t_token **word_tokens,
			t_token **token, int *word_count)
{
	t_token	*new_word;

	new_word = malloc(sizeof(t_token));
	if (!new_word)
		return (0);
	new_word->type = TOKEN_WORD;
	new_word->value = ft_strdup((*token)->value);
	new_word->next = *word_tokens;
	*word_tokens = new_word;
	(*word_count)++;
	*token = (*token)->next;
	return (1);
}

static int	build_argv(t_nodes *node, t_token *word_tokens, int word_count)
{
	int		i;
	t_token	*next;

	node->argv = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!node->argv)
		return (0);
	i = word_count - 1;
	while (word_tokens)
	{
		node->argv[i--] = ft_strdup(word_tokens->value);
		next = word_tokens->next;
		free(word_tokens->value);
		free(word_tokens);
		word_tokens = next;
	}
	node->argv[word_count] = NULL;
	node->cmd = ft_strdup(node->argv[0]);
	return (1);
}

static void	cleanup_word_tokens(t_token *word_tokens)
{
	t_token	*next;

	while (word_tokens)
	{
		next = word_tokens->next;
		free(word_tokens->value);
		free(word_tokens);
		word_tokens = next;
	}
}

static t_nodes	*process_token_loop(t_nodes *node, t_token **token,
			t_token **word_tokens, int *word_count)
{
	while (*token && ((*token)->type == TOKEN_WORD || \
		(*token)->type == TOKEN_REDIR_IN || \
		(*token)->type == TOKEN_REDIR_OUT || \
		(*token)->type == TOKEN_APPEND || (*token)->type == TOKEN_HEREDOC))
	{
		if ((*token)->type == TOKEN_WORD)
		{
			if (!process_word_token(word_tokens, token, word_count))
			{
				cleanup_word_tokens(*word_tokens);
				free_node(node);
				return (NULL);
			}
		}
		else if (!handle_redirections(node, token))
		{
			cleanup_word_tokens(*word_tokens);
			free_node(node);
			return (NULL);
		}
	}
	return (node);
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
	word_tokens = NULL;
	word_count = 0;
	node = process_token_loop(node, token, &word_tokens, &word_count);
	if (!node)
		return (NULL);
	if (word_count > 0 && !build_argv(node, word_tokens, word_count))
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}
