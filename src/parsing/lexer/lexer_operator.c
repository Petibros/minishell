/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:42:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:42:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_token	*handle_redirect(char **input, char curr, char next)
{
	if (curr == '<' && next == '<')
	{
		(*input) += 2;
		return (create_token(TOKEN_HEREDOC, ft_strdup("<<")));
	}
	else if (curr == '>' && next == '>')
	{
		(*input) += 2;
		return (create_token(TOKEN_APPEND, ft_strdup(">>")));
	}
	else if (curr == '<')
	{
		(*input)++;
		return (create_token(TOKEN_REDIR_IN, ft_strdup("<")));
	}
	(*input)++;
	return (create_token(TOKEN_REDIR_OUT, ft_strdup(">")));
}

static t_token	*handle_logical(char **input, char curr, char next)
{
	if (curr == '|' && next == '|')
	{
		(*input) += 2;
		return (create_token(TOKEN_OR, ft_strdup("||")));
	}
	else if (curr == '&' && next == '&')
	{
		(*input) += 2;
		return (create_token(TOKEN_AND, ft_strdup("&&")));
	}
	else if (curr == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, ft_strdup("|")));
	}
	return (NULL);
}

static t_token	*handle_lexer_parentheses(char **input, char curr)
{
	if (curr == '(')
	{
		(*input)++;
		return (create_token(TOKEN_LPAREN, ft_strdup("(")));
	}
	(*input)++;
	return (create_token(TOKEN_RPAREN, ft_strdup(")")));
}

t_token	*handle_lexer_operator(char **input)
{
	char	curr;
	char	next;

	curr = **input;
	next = *(*input + 1);
	if (curr == '<' || curr == '>')
		return (handle_redirect(input, curr, next));
	if (curr == '|' || curr == '&')
		return (handle_logical(input, curr, next));
	if (curr == '(' || curr == ')')
		return (handle_lexer_parentheses(input, curr));
	return (NULL);
}
