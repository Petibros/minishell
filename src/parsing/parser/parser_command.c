/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*init_command_parsing(void)
{
	t_nodes	*node;

	node = create_node();
	if (!node)
		return (NULL);
	return (node);
}

static t_nodes	*process_and_build_command(t_cmd_ctx *ctx)
{
	if (!process_command_token(ctx))
	{
		free_node(ctx->node);
		return (NULL);
	}
	if (*ctx->word_count > 0
		&& !build_argv(ctx->node, *ctx->word_tokens, *ctx->word_count))
	{
		free_node(ctx->node);
		return (NULL);
	}
	return (ctx->node);
}

t_nodes	*parse_command(t_token **token, char **envp, t_vars *vars)
{
	t_nodes		*node;
	t_token		*word_tokens;
	int			word_count;
	t_cmd_ctx	ctx;

	if (!*token)
		return (NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_parentheses(token, envp, vars));
	node = init_command_parsing();
	if (!node)
		return (NULL);
	word_tokens = NULL;
	word_count = 0;
	ctx = (t_cmd_ctx){node, token, &word_tokens, &word_count, envp};
	return (process_and_build_command(&ctx));
}
