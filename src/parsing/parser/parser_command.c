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

t_nodes	*parse_command(t_token **token, char **envp)
{
	t_nodes		*node;
	t_token		*word_tokens;
	int			word_count;
	t_cmd_ctx	ctx;

	node = create_parser_node();
	if (!node)
		return (NULL);
	word_tokens = NULL;
	word_count = 0;
	ctx = (t_cmd_ctx){node, token, &word_tokens, &word_count, envp};
	while (*token && ((*token)->type == TOKEN_WORD
			|| (*token)->type == TOKEN_REDIR_IN
			|| (*token)->type == TOKEN_REDIR_OUT
			|| (*token)->type == TOKEN_APPEND
			|| (*token)->type == TOKEN_HEREDOC))
	{
		if (!process_command_token(&ctx))
			return (NULL);
	}
	if (word_count > 0 && !build_argv(node, word_tokens, word_count))
		return (NULL);
	return (node);
}
