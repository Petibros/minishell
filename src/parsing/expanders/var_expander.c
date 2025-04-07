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
		/* When tmp is NULL (variable not found), we just continue with the existing result
		   instead of setting it to NULL, which matches bash behavior */
	}
	else
		ctx->result = handle_regular_char(ctx->str, ctx->i, ctx->result);
}

char	*expand_variables(char *str, int exit_status, char **envp)
{
	t_quote_ctx	*ctx;
	char		*result;
	char		*final_result;

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
	
	/* Handle non-existent variables like bash */
	if (result && ft_strchr(str, '$'))
	{
		char *var_start = ft_strchr(str, '$');
		if (var_start && (ft_isalpha(*(var_start + 1)) || *(var_start + 1) == '_'))
		{
			/* Check if the variable name is still in the result (not expanded) */
			char *var_name = var_start + 1;
			int var_len = 0;
			while (var_name[var_len] && (ft_isalnum(var_name[var_len]) || var_name[var_len] == '_'))
				var_len++;
				
			char *var_in_result = ft_strnstr(result, var_start, ft_strlen(result));
			if (var_in_result)
			{
				/* Variable wasn't expanded, remove it from the result */
				char *before = ft_substr(result, 0, var_in_result - result);
				char *after = ft_strdup(var_in_result + var_len + 1);
				
				final_result = ft_strjoin(before, after);
				free(before);
				free(after);
				free(result);
				return (final_result);
			}
		}
	}
	
	return (result);
}
