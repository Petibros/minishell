/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_validation_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:24:55 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:24:55 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	is_dollar_operator(const char *value)
{
	return (value && value[0] == '$'
		&& (value[1] == '>' || value[1] == '<') && value[2] == '\0');
}

int	validate_redirection(t_token *next)
{
	if (!next || next->type != TOKEN_WORD)
		return (0);
	if (next->next && next->next->type == TOKEN_LPAREN)
		return (0);
	if (next->type == TOKEN_WORD && is_dollar_operator(next->value))
		return (0);
	return (1);
}
