/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

static void	process_char(t_quote_ctx *ctx, char *tmp)
{
	if (ctx->str[*(ctx->i)] == '\'')
		ctx->result = handle_single_quote(ctx->str, ctx->i, ctx->result);
	else if (ctx->str[*(ctx->i)] == '"')
		ctx->result = handle_double_quote_char(ctx);
	else if (ctx->str[*(ctx->i)] == '$')
	{
		tmp = expand_env_var(ctx->str, ctx->i, ctx->exit_status, ctx->envp);
		if (tmp)
		{
			ctx->result = ft_strjoin_free(ctx->result, tmp);
			free(tmp);
		}
		else
		{
			free(ctx->result);
			ctx->result = NULL;
		}
	}
	else
		ctx->result = handle_regular_char(ctx->str, ctx->i, ctx->result);
}

char	*expand_variables(char *str, int exit_status, char **envp)
{
	t_quote_ctx	*ctx;
	char		*result;

	ctx = init_quote_context(str, exit_status, envp);
	if (!ctx)
		return (NULL);
	while (str[*(ctx->i)] && ctx->result)
		process_char(ctx, NULL);
	result = ctx->result;
	if (result && result[0] == '\0')
	{
		free(result);
		result = NULL;
	}
	free(ctx->i);
	free(ctx);
	return (result);
}
