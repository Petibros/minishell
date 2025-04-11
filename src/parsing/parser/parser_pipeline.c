/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/04/11 22:21:10 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*handle_subshell(t_token **token, char **envp)
{
	t_nodes	*node;
	t_nodes	*inner_cmd;

	*token = (*token)->next;
	inner_cmd = parse_and_or(token, envp);
	if (!inner_cmd)
		return (NULL);
	if (!*token || (*token)->type != TOKEN_RPAREN)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	*token = (*token)->next;
	node = create_parser_node();
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

static t_nodes	*get_next_command(t_token **token, char **envp)
{
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_subshell(token, envp));
	return (parse_command(token, envp));
}

t_nodes	*parse_pipeline(t_token **token, char **envp)
{
	t_nodes	*first_cmd;
	t_nodes	*current;
	t_nodes	*next_cmd;

	first_cmd = get_next_command(token, envp);
	if (!first_cmd)
		return (NULL);
	current = first_cmd;
	while (*token && (*token)->type == TOKEN_PIPE)
	{
		*token = (*token)->next;
		next_cmd = get_next_command(token, envp);
		if (!next_cmd)
		{
			free_node(first_cmd);
			return (NULL);
		}
		current->is_operator = 1;
		current->operator_type = TOKEN_PIPE;
		current->right = next_cmd;
		current = next_cmd;
	}
	return (first_cmd);
}
