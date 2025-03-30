/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:09:00 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 17:09:00 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"
#include "libft.h"

static char	*get_var_value(char *var_name, int exit_status, char **envp)
{
	char	*var_value;

	if (!ft_strncmp(var_name, "?", 2))
		return (ft_itoa(exit_status));
	if (!ft_strncmp(var_name, "_", 2))
	{
		var_value = ft_getenv(envp, "_");
		if (!var_value)
			return (NULL);
		return (ft_strdup(var_value));
	}
	var_value = ft_getenv(envp, var_name);
	if (!var_value)
		return (NULL);
	return (ft_strdup(var_value));
}

static char	*get_var_name(char *str, int *i)
{
	int		len;
	char	*var_name;

	len = 0;
	if (str[*i] == '_' || ft_isalpha(str[*i]))
	{
		len++;
		while ((ft_isalnum(str[*i + len]) || str[*i + len] == '_') && str[*i + len] != '$')
			len++;
	}
	if (len == 0)
		return (ft_strdup(""));
	var_name = ft_substr(str, *i, len);
	*i += len;
	return (var_name);
}

char	*expand_env_var(char *str, int *i, int exit_status, char **envp)
{
	char	*var_name;
	char	*result;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	if (str[*i] == '\'')
	{
		(*i)--;
		return (ft_strdup(""));
	}
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
	{
		if (str[*i] == '"' && *(i) == 1)
			return (ft_strdup(""));
		return (ft_strdup("$"));
	}
	var_name = get_var_name(str, i);
	result = get_var_value(var_name, exit_status, envp);
	free(var_name);
	if (!result)
		return (NULL);
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
