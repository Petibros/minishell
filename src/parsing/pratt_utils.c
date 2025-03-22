/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:11:38 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 03:11:38 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	get_precedence(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (1);
	if (type == TOKEN_AND || type == TOKEN_OR)
		return (2);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || 
		type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (3);
	return (0);
}

int	count_word_tokens(t_token **token)
{
	int		count;
	t_token	*current;

	count = 0;
	current = *token;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	advance_token(t_token **token, int count)
{
	int	i;

	i = 0;
	while (i < count && *token)
	{
		*token = (*token)->next;
		i++;
	}
}

t_nodes	*handle_parentheses(t_token **token)
{
	t_nodes	*node;

	if (!*token || (*token)->type != TOKEN_LPAREN)
		return (NULL);
	*token = (*token)->next;
	node = parse_expression(token, 0);
	if (*token && (*token)->type == TOKEN_RPAREN)
		*token = (*token)->next;
	return (node);
}
