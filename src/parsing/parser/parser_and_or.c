/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_and_or.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/04/11 22:21:10 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*handle_subshell_cmd(t_token **token, char **envp, t_vars *vars)
{
	t_nodes	*node;
	t_nodes	*inner_cmd;

	*token = (*token)->next;
	inner_cmd = parse_and_or(token, envp, vars);
	if (!inner_cmd)
		return (NULL);
	if (!*token || (*token)->type != TOKEN_RPAREN)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	node = create_node();
	if (!node)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	node->is_operator = 1;
	node->operator_type = TOKEN_SUBSHELL;
	node->right = inner_cmd;
	*token = (*token)->next;
	return (node);
}

static t_nodes	*get_next_and_or_cmd(t_token **token, char **envp, t_vars *vars)
{
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_subshell_cmd(token, envp, vars));
	return (parse_pipeline(token, envp, vars));
}

static int	process_next_cmd(t_and_or_ctx *ctx)
{
	t_nodes	*next_cmd;

	next_cmd = get_next_and_or_cmd(ctx->token, ctx->envp, ctx->vars);
	if (!next_cmd)
		return (0);
	ctx->current->is_operator = 1;
	ctx->current->operator_type = ctx->op_type;
	ctx->current->right = next_cmd;
	return (1);
}

t_nodes	*parse_and_or(t_token **token, char **envp, t_vars *vars)
{
	t_nodes			*first_cmd;
	t_and_or_ctx	ctx;

	first_cmd = parse_pipeline(token, envp, vars);
	if (!first_cmd)
		return (NULL);
	ctx.current = first_cmd;
	while (*token && ((*token)->type == TOKEN_AND
			|| (*token)->type == TOKEN_OR))
	{
		ctx.op_type = (*token)->type;
		ctx.token = token;
		ctx.envp = envp;
		ctx.vars = vars;
		*token = (*token)->next;
		if (!process_next_cmd(&ctx))
		{
			free_node(first_cmd);
			return (NULL);
		}
		ctx.current = ctx.current->right;
	}
	return (first_cmd);
}
