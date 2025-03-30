/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

static char	*handle_dollar_in_dquote(t_quote_ctx *ctx)
{
	char	*tmp;

	tmp = expand_env_var(ctx->str, ctx->i, ctx->exit_status, ctx->envp);
	if (tmp)
	{
		ctx->result = ft_strjoin_free(ctx->result, tmp);
		free(tmp);
	}
	return (ctx->result);
}

static char	*process_double_quote_content(t_quote_ctx *ctx)
{
	int		start;

	start = *(ctx->i);
	while (ctx->str[*(ctx->i)] && ctx->str[*(ctx->i)] != '"')
	{
		if (ctx->str[*(ctx->i)] == '$')
		{
			if (start != *(ctx->i))
				ctx->result = append_substring(ctx->result, ctx->str, \
												start, *(ctx->i));
			(*(ctx->i))++; // Skip the $ character
			ctx->result = handle_dollar_in_dquote(ctx);
			start = *(ctx->i);
		}
		else
			(*(ctx->i))++;
	}
	if (ctx->str[*(ctx->i)] == '"' && start != *(ctx->i))
	{
		ctx->result = append_substring(ctx->result, ctx->str, start, *(ctx->i));
		(*(ctx->i))++;
	}
	return (ctx->result);
}

char	*handle_double_quote_char(t_quote_ctx *ctx)
{
	(*(ctx->i))++;
	return (process_double_quote_content(ctx));
}
