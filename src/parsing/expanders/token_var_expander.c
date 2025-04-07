/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_var_expander.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:52:15 by npapash           #+#    #+#             */
/*   Updated: 2025/04/07 02:52:15 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

/*
 * Expands environment variables in a token's value
 */
static void	expand_token_value(t_token *token, int exit_status, char **envp)
{
	char	*expanded;

	if (!token || !token->value || !ft_strchr(token->value, '$'))
		return ;
	expanded = expand_variables(token->value, exit_status, envp);
	if (expanded)
	{
		free(token->value);
		token->value = expanded;
	}
}

/*
 * Recursively expands environment variables in all tokens
 */
void	expand_variables_in_tokens(t_token *tokens, int exit_status, char **envp)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			expand_token_value(current, exit_status, envp);
		current = current->next;
	}
}
