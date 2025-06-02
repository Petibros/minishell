/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_parser_atom.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:39:23 by npapash           #+#    #+#             */
/*   Updated: 2025/06/02 14:23:55 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

static t_nodes	*process_token_loop(t_token_loop_ctx *ctx)
{
	while (*(ctx->token) && ((*(ctx->token))->type == TOKEN_WORD
			|| (*(ctx->token))->type == TOKEN_REDIR_IN
			|| (*(ctx->token))->type == TOKEN_REDIR_OUT
			|| (*(ctx->token))->type == TOKEN_APPEND
			|| (*(ctx->token))->type == TOKEN_HEREDOC))
	{
		if ((*(ctx->token))->type == TOKEN_WORD)
		{
			if (!process_word_token(ctx->word_tokens, ctx->token,
					ctx->word_count))
			{
				cleanup_word_tokens(*(ctx->word_tokens));
				free_node(ctx->node);
				return (NULL);
			}
		}
		else if (!handle_redirections(ctx->node, ctx->token))
		{
			cleanup_word_tokens(*(ctx->word_tokens));
			free_node(ctx->node);
			return (NULL);
		}
	}
	return (ctx->node);
}

t_nodes	*parse_atom(t_token **token, char **envp, t_vars *vars)
{
	t_nodes				*node;
	t_token				*word_tokens;
	int					word_count;
	t_token_loop_ctx	ctx;

	if (!*token)
		return (NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_parentheses(token, envp, vars));
	node = create_node();
	if (!node)
		return (NULL);
	word_tokens = NULL;
	word_count = 0;
	ctx = (t_token_loop_ctx){node, token, &word_tokens, &word_count, envp};
	node = process_token_loop(&ctx);
	if (!node)
		return (NULL);
	if (word_count > 0 && !build_argv(node, word_tokens, word_count))
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}
