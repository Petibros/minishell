/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:09:00 by npapash           #+#    #+#             */
/*   Updated: 2025/04/11 22:39:43 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"
#include "libft.h"

char	*handle_underscore_case(char *var_name, int exit_status, char **envp)
{
	char	*underscore_pos;
	char	*base_var;
	char	*base_value;
	char	*result;

	underscore_pos = ft_strchr(var_name, '_');
	if (!underscore_pos || underscore_pos == var_name)
		return (NULL);
	base_var = ft_substr(var_name, 0, underscore_pos - var_name);
	base_value = get_var_value(base_var, exit_status, envp);
	result = NULL;
	if (base_value)
		result = ft_strjoin(base_value, underscore_pos);
	free(base_value);
	free(base_var);
	return (result);
}

char	*expand_env_var(char *str, int *i, int exit_status, char **envp)
{
	char	*var_name;
	char	*result;
	char	*special_case;

	special_case = handle_special_var_cases(str, i, exit_status);
	if (special_case)
		return (special_case);
	var_name = get_var_name(str, i);
	result = get_var_value(var_name, exit_status, envp);
	if (!result && ft_strchr(var_name, '_'))
		result = handle_underscore_case(var_name, exit_status, envp);
	free(var_name);
	return (result);
}

t_quote_ctx	*init_quote_context(char *str, int exit_status, char **envp)
{
	t_quote_ctx	*ctx;

	ctx = malloc(sizeof(t_quote_ctx));
	if (!ctx)
		return (NULL);
	ctx->str = str;
	ctx->i = malloc(sizeof(int));
	if (!ctx->i)
	{
		free(ctx);
		return (NULL);
	}
	*(ctx->i) = 0;
	ctx->result = ft_strdup("");
	ctx->exit_status = exit_status;
	ctx->envp = envp;
	return (ctx);
}
