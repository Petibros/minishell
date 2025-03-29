/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:22:30 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 17:22:30 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*create_word_token(t_token *src)
{
	t_token	*new_word;

	new_word = malloc(sizeof(t_token));
	if (!new_word)
		return (NULL);
	new_word->type = TOKEN_WORD;
	new_word->value = ft_strdup(src->value);
	new_word->next = NULL;
	if (!new_word->value)
	{
		free(new_word);
		return (NULL);
	}
	return (new_word);
}

int	process_word_token(t_token **word_tokens, t_token **token, int *word_count)
{
	t_token	*new_word;

	new_word = create_word_token(*token);
	if (!new_word)
		return (0);
	if (!*word_tokens)
		*word_tokens = new_word;
	else
	{
		new_word->next = *word_tokens;
		*word_tokens = new_word;
	}
	(*word_count)++;
	*token = (*token)->next;
	return (1);
}

int	process_command_token(t_cmd_ctx *ctx)
{
	if ((*(ctx->token))->type == TOKEN_WORD)
	{
		if (!process_word_token(ctx->word_tokens, ctx->token, ctx->word_count))
			return (0);
	}
	else if (!handle_redirections(ctx->node, ctx->token, ctx->envp))
		return (0);
	return (1);
}

int	build_argv(t_nodes *node, t_token *word_tokens, int word_count)
{
	int		i;
	t_token	*next;

	node->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!node->argv)
		return (0);
	i = word_count - 1;
	while (word_tokens)
	{
		node->argv[i] = ft_strdup(word_tokens->value);
		next = word_tokens->next;
		free(word_tokens->value);
		free(word_tokens);
		word_tokens = next;
		i--;
	}
	node->argv[word_count] = NULL;
	return (1);
}
