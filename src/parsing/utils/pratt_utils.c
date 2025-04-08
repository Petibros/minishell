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
	if (type == TOKEN_OR)
		return (3);
	if (type == TOKEN_PIPE)
		return (2);
	if (type == TOKEN_AND)
		return (1);
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

t_nodes	*handle_parentheses(t_token **token, char **envp)
{
	t_nodes	*node;
	t_nodes	*inner_cmd;

	if (!*token || (*token)->type != TOKEN_LPAREN)
		return (NULL);
	*token = (*token)->next;
	inner_cmd = parse_expression(token, 0, envp);
	if (!inner_cmd)
		return (NULL);
	if (!*token || (*token)->type != TOKEN_RPAREN)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	*token = (*token)->next;
	node = create_node();
	if (!node)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	node->is_operator = 1;
	node->operator_type = TOKEN_SUBSHELL;
	node->right = inner_cmd;
	return (node);
}
