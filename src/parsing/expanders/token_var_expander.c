/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_var_expander.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <npapashv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:52:15 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 21:49:31 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*handle_dollar_following(char *after_var, char *token_value,
			char **envp);
char	*handle_var_value(char *token_value, int var_len, char **envp);
char	*handle_dollar_continuation(char *token_value, char *var_value,
			int var_len, char **envp);

static char	*extract_var_and_get_value(char *token_value, int *var_len,
			char **envp)
{
	char	*var_name;
	char	*var_value;

	*var_len = 1;
	while (token_value[*var_len]
		&& (ft_isalnum(token_value[*var_len]) || token_value[*var_len] == '_'))
		(*var_len)++;
	var_name = ft_substr(token_value, 1, *var_len - 1);
	if (!var_name)
	{
		free(token_value);
		return (NULL);
	}
	var_value = ft_getenv(envp, var_name);
	free(var_name);
	return (var_value);
}

char	*process_var_token(char *token_value, char **envp)
{
	char	*var_value;
	char	*result;
	int		var_len;

	if (!token_value || !token_value[0])
		return (NULL);
	var_value = extract_var_and_get_value(token_value, &var_len, envp);
	if (!var_value)
		return (handle_var_value(token_value, var_len, envp));
	if (token_value[var_len] == '$')
	{
		result = handle_dollar_continuation(token_value, var_value,
				var_len, envp);
		free(token_value);
		return (result);
	}
	free(token_value);
	return (ft_strdup(var_value));
}

void	expand_token_value(t_token *token, int exit_status, char **envp)
{
	char	*expanded;

	if (!token || !token->value || !ft_strchr(token->value, '$'))
		return ;
	if (token->value[0] == '$' && token->value[1] == '*'
		&& token->value[2] == '\0')
	{
		free(token->value);
		token->value = ft_strdup("");
		return ;
	}
	if (token->value[0] == '$' && (ft_isalpha(token->value[1])
			|| token->value[1] == '_'))
	{
		token->value = process_var_token(token->value, envp);
		return ;
	}
	expanded = expand_variables(token->value, exit_status, envp);
	if (expanded)
	{
		free(token->value);
		token->value = expanded;
	}
}

void	expand_variables_in_tokens(t_token **tokens, int exit_status,
						char **envp)
{
	t_token					*current;
	t_token_processor_ctx	ctx;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	ctx.prev = NULL;
	ctx.exit_status = exit_status;
	ctx.envp = envp;
	while (current)
	{
		ctx.next = current->next;
		current = process_token(tokens, current, &ctx);
		if (current)
			ctx.prev = current;
		current = ctx.next;
	}
}
