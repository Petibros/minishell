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
		else if (ctx->str[*(ctx->i) + 1] == '\0')
		{
			tmp = ft_strdup("$");
			ctx->result = ft_strjoin_free(ctx->result, tmp);
			free(tmp);
			(*(ctx->i))++;
		}
	}
	else
		ctx->result = handle_regular_char(ctx->str, ctx->i, ctx->result);
}

static char	*extract_variable_part(char *result, char *var_in_result,
	int var_len)
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

static int	get_var_length(char *var_name)
{
	int	var_len;

	var_len = 0;
	while (var_name[var_len] && (ft_isalnum(var_name[var_len])
			|| var_name[var_len] == '_'))
		var_len++;
	return (var_len);
}

static char	*post_process_expansion(char *result, char *str)
{
	char	*start;
	char	*var_in_result;
	int		var_len;

	if (!result || !ft_strchr(str, '$'))
		return (result);
	start = ft_strchr(str, '$');
	if (!start || (!ft_isalpha(*(start + 1)) && *(start + 1) != '_'))
		return (result);
	var_len = get_var_length(start + 1);
	var_in_result = ft_strnstr(result, start, ft_strlen(result));
	if (!var_in_result)
		return (result);
	return (extract_variable_part(result, var_in_result, var_len));
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
	return (post_process_expansion(result, str));
}
