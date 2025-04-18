/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_nodes	*parse_command(t_token **token, char **envp, t_vars *vars)
{
	t_nodes	*node;
	t_token	*word_tokens;
	int		word_count;
	t_cmd_ctx	ctx;

	if (!*token)
		return (NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (handle_parentheses(token, envp, vars));
	node = create_parser_node();
	if (!node)
		return (NULL);
	word_tokens = NULL;
	word_count = 0;
	ctx = (t_cmd_ctx){node, token, &word_tokens, &word_count, envp};
	if (!process_command_token(&ctx))
	{
		free_node(node);
		return (NULL);
	}
	if (word_count > 0 && !build_argv(node, word_tokens, word_count))
	{
		free_node(node);
		return (NULL);
	}
	return (node);
}
