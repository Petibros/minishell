/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_validation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:24:55 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:24:55 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	validate_parentheses_content(t_token *current)
{
	current = current->next;
	if (!current || current->type == TOKEN_RPAREN)
		return (0);
	while (current && current->type != TOKEN_RPAREN)
	{
		if (is_redirection(current->type)
			&& !validate_redirection(current->next))
			return (0);
		if (current->type == TOKEN_WORD
			&& is_dollar_operator(current->value))
			return (0);
		current = current->next;
	}
	if (!current || current->type != TOKEN_RPAREN)
		return (0);
	return (1);
}

static int	validate_pipe_sequence(t_token *current, t_token *next)
{
	if (current->type == TOKEN_PIPE && (!next || next->type == TOKEN_PIPE))
		return (0);
	return (1);
}

static int	validate_token(t_token *current, t_token *next)
{
	if (!validate_pipe_sequence(current, next))
		return (0);
	if (is_redirection(current->type) && !validate_redirection(next))
		return (0);
	if (current->type == TOKEN_LPAREN
		&& !validate_parentheses_content(current))
		return (0);
	if (current->type == TOKEN_WORD && is_dollar_operator(current->value))
		return (0);
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (0);
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (0);
	while (current)
	{
		next = current->next;
		if (!validate_token(current, next))
			return (0);
		current = next;
	}
	return (1);
}
