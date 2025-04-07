/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_wildcard_expander.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:52:15 by npapash           #+#    #+#             */
/*   Updated: 2025/04/07 02:52:15 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "wildcard_expander.h"

/*
 * Expands wildcard patterns in a token's value
 * Note: This function is simplified as wildcard expansion typically
 * creates multiple tokens from a single pattern, which is complex to handle
 * at the token level. For full functionality, consider expanding after parsing.
 */
static void	expand_token_wildcard(t_token *token)
{
	int		count;
	char	**matches;
	char	*joined;

	if (!token || !token->value || !has_unquoted_wildcard(token->value))
		return ;
	count = count_matching_entries(token->value);
	if (count <= 0)
		return ;
	matches = collect_matching_entries(token->value, count);
	if (!matches)
		return ;
	joined = ft_strjoin_array(matches, " ");
	if (joined)
	{
		free(token->value);
		token->value = joined;
	}
	free_array(matches);
}

/*
 * Recursively expands wildcards in all tokens
 */
void	expand_wildcards_in_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			expand_token_wildcard(current);
		current = current->next;
	}
}
