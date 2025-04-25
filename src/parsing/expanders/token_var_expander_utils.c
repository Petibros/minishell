/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_var_expander_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <npapashv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:43:43 by npapashv          #+#    #+#             */
/*   Updated: 2025/04/25 21:43:43 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*handle_dollar_following(char *after_var, char *token_value,
			char **envp)
{
	char	*new_value;
	char	*expanded;

	if (*after_var == '$')
	{
		if (*(after_var + 1) == '\0')
		{
			new_value = ft_strdup("$");
			free(token_value);
			return (new_value);
		}
		expanded = process_var_token(ft_strdup(after_var), envp);
		if (!expanded)
			expanded = ft_strdup("");
		free(token_value);
		return (expanded);
	}
	new_value = ft_strdup(after_var);
	free(token_value);
	return (new_value);
}

char	*handle_var_value(char *token_value, int var_len, char **envp)
{
	char	*after_var;

	if (token_value[var_len])
	{
		after_var = token_value + var_len;
		return (handle_dollar_following(after_var, token_value, envp));
	}
	free(token_value);
	return (ft_strdup(""));
}

char	*handle_dollar_continuation(char *token_value, char *var_value,
					int var_len, char **envp)
{
	char	*next_part;
	char	*result;

	if (token_value[var_len + 1] == '\0')
		return (ft_strjoin(var_value, "$"));
	next_part = process_var_token(ft_strdup(token_value + var_len), envp);
	if (next_part)
	{
		result = ft_strjoin(var_value, next_part);
		free(next_part);
		return (result);
	}
	return (ft_strdup(var_value));
}
