/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 06:53:09 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 06:53:09 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*handle_dollar_sign(t_quote_ctx *ctx)
{
	char	*tmp;

	tmp = expand_env_var(ctx->str, &ctx->i, ctx->exit_status, ctx->envp);
	if (tmp)
	{
		ctx->result = ft_strjoin_free(ctx->result, tmp);
		free(tmp);
	}
	else if (ctx->str[ctx->i + 1] == '\0')
	{
		tmp = ft_strdup("$");
		ctx->result = ft_strjoin_free(ctx->result, tmp);
		free(tmp);
		ctx->i++;
	}
	return (ctx->result);
}

char	*handle_in_squote(t_quote_ctx *ctx)
{
	char	*tmp;

	tmp = ft_substr(ctx->str, ctx->i, 1);
	if (tmp)
	{
		ctx->result = ft_strjoin_free(ctx->result, tmp);
		free(tmp);
	}
	ctx->i++;
	return (ctx->result);
}

char	*extract_variable_part(char *result, char *var_in_result, int var_len)
{
	char	*before;
	char	*after;
	char	*final_result;

	before = ft_substr(result, 0, var_in_result - result);
	after = ft_strdup(var_in_result + var_len + 1);
	final_result = ft_strjoin(before, after);
	free(before);
	free(after);
	free(result);
	return (final_result);
}

int	get_var_length(char *var_name)
{
	int	var_len;

	var_len = 0;
	while (var_name[var_len] && (ft_isalnum(var_name[var_len])
			|| var_name[var_len] == '_'))
		var_len++;
	return (var_len);
}
