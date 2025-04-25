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
		if (tmp[0] != '\0' || ctx->str[*(ctx->i)] == '"')
			ctx->result = ft_strjoin_free(ctx->result, tmp);
		free(tmp);
	}
	return (ctx->result);
}

static char	*handle_closing_quote(t_quote_ctx *ctx, int start)
{
	char	*tmp;

	if (ctx->str[*(ctx->i)] == '"')
	{
		if (start != *(ctx->i))
			ctx->result = append_substring(ctx->result, ctx->str,
					start, *(ctx->i));
		tmp = ft_strjoin_free(ctx->result, "\"");
		if (!tmp)
			return (NULL);
		ctx->result = tmp;
		(*(ctx->i))++;
	}
	return (ctx->result);
}

static int	is_valid_dollar_sequence(t_quote_ctx *ctx)
{
	return (ft_isalpha(ctx->str[*(ctx->i) + 1])
			|| ctx->str[*(ctx->i) + 1] == '_'
			|| ctx->str[*(ctx->i) + 1] == '?');
}

static char	*process_double_quote_content(t_quote_ctx *ctx)
{
	int		start;
	char	*tmp;

	tmp = ft_strjoin_free(ctx->result, "\"");
	if (!tmp)
		return (NULL);
	ctx->result = tmp;
	start = *(ctx->i);
	while (ctx->str[*(ctx->i)] && ctx->str[*(ctx->i)] != '"')
	{
		if (ctx->str[*(ctx->i)] == '$' && is_valid_dollar_sequence(ctx))
		{
			if (start != *(ctx->i))
				ctx->result = append_substring(ctx->result, ctx->str,
						start, *(ctx->i));
			ctx->result = handle_dollar_in_dquote(ctx);
			start = *(ctx->i);
		}
		else
			(*(ctx->i))++;
	}
	return (handle_closing_quote(ctx, start));
}

char	*handle_double_quote_char(t_quote_ctx *ctx)
{
	(*(ctx->i))++;
	return (process_double_quote_content(ctx));
}
