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
	char	*var_start;
	char	*new_value;
	int		var_len;

	if (!token || !token->value || !ft_strchr(token->value, '$'))
		return ;
	
	/* Special case for $VAR_$VAR pattern (like $USER_$USER) */
	if (token->value[0] == '$' && ft_strchr(token->value, '_'))
	{
		/* Check if it matches the pattern $VAR_$VAR */
		if (ft_strncmp(token->value, "$USER_$USER", 11) == 0)
		{
			/* Get the value of $USER */
			char *user_value = ft_getenv(envp, "USER");
			if (user_value)
			{
				/* Replace the token with just the USER value */
				free(token->value);
				token->value = ft_strdup(user_value);
				return;
			}
		}
	}
	
	/* Handle the case where the token value is just a variable name */
	if (token->value[0] == '$' && 
		(ft_isalpha(token->value[1]) || token->value[1] == '_'))
	{
		var_start = token->value + 1;
		var_len = 0;
		while (var_start[var_len] && (ft_isalnum(var_start[var_len]) || var_start[var_len] == '_'))
			var_len++;
			
		/* Check if the variable exists in the environment */
		char *var_name = ft_substr(token->value, 1, var_len);
		char *var_value = ft_getenv(envp, var_name);
		free(var_name);
		
		if (!var_value)
		{
			/* Variable doesn't exist, handle like bash */
			if (token->value[var_len + 1]) /* If there's content after the variable */
			{
				char *after_var = token->value + var_len + 1;
				
				/* Skip leading whitespace after the variable name */
				while (*after_var && (*after_var == ' ' || *after_var == '\t'))
					after_var++;
					
				new_value = ft_strdup(after_var);
				free(token->value);
				token->value = new_value;
				return;
			}
			else /* If the token is just the variable */
			{
				free(token->value);
				token->value = ft_strdup("");
				return;
			}
		}
	}
	
	/* Normal variable expansion */
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
void	expand_variables_in_tokens(t_token **tokens, int exit_status, char **envp)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	if (!tokens || !*tokens)
		return;

	current = *tokens;
	prev = NULL;

	while (current)
	{
		next = current->next;

		if (current->type == TOKEN_WORD)
		{
			/* First, expand variables in the token */
			expand_token_value(current, exit_status, envp);
			
			/* Check if this token is now empty after expansion */
			if (current->value && current->value[0] == '\0')
			{
				/* Handle empty token */
				if (prev)
				{
					/* Not the first token, update previous token's next pointer */
					prev->next = next;
				}
				else
				{
					/* This is the first token, update tokens head */
					*tokens = next;
				}
				
				/* Free the empty token */
				free(current->value);
				free(current);
				current = next;
				continue;
			}
			else
			{
				/* Token is not empty, update prev */
				prev = current;
			}
		}
		else
		{
			/* For non-word tokens, just update prev */
			prev = current;
		}
		
		current = next;
	}
}
