/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:09:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/18 20:09:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	is_operator_char(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>' || c == '(' || c == ')');
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

static t_token	*handle_operator(char **input)
{
	char	curr;
	char	next;

	curr = **input;
	next = *(*input + 1);
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
	else if (curr == '<' && next == '<')
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
	else if (curr == '>')
	{
		(*input)++;
		return (create_token(TOKEN_REDIR_OUT, ft_strdup(">")));
	}
	else if (curr == '|')
	{
		(*input)++;
		return (create_token(TOKEN_PIPE, ft_strdup("|")));
	}
	else if (curr == '(')
	{
		(*input)++;
		return (create_token(TOKEN_LPAREN, ft_strdup("(")));
	}
	else if (curr == ')')
	{
		(*input)++;
		return (create_token(TOKEN_RPAREN, ft_strdup(")")));
	}
	return (NULL);
}

static char	*get_word(char **input)
{
	int		len;
	char	*word;
	char	quote;
	int		i;

	len = 0;
	quote = 0;
	while ((*input)[len] && (!is_operator_char((*input)[len]) || quote) &&
			(!is_whitespace((*input)[len]) || quote))
	{
		if ((*input)[len] == '\'' || (*input)[len] == '\"')
		{
			if (!quote)
				quote = (*input)[len];
			else if (quote == (*input)[len])
				quote = 0;
		}
		len++;
	}
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = (*input)[i];
		i++;
	}
	word[i] = '\0';
	*input += len;
	return (word);
}

t_token	*get_next_token(char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
	if (!**input)
		return (create_token(TOKEN_EOF, NULL));
	if (is_operator_char(**input))
		return (handle_operator(input));
	return (create_token(TOKEN_WORD, get_word(input)));
}

t_token	*lexer(char *input)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;

	head = NULL;
	current = NULL;
	while (*input)
	{
		new_token = get_next_token(&input);
		if (!new_token)
			return (NULL);
		if (new_token->type == TOKEN_EOF)
		{
			free(new_token);
			break ;
		}
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	return (head);
}
