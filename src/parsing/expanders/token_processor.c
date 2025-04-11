/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:00:00 by npapash           #+#    #+#             */
/*   Updated: 2025/04/11 22:00:00 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

void	handle_empty_token(t_token **tokens, t_token *current,
	t_token *prev, t_token *next)
{
	if (prev)
		prev->next = next;
	else
		*tokens = next;
	free(current->value);
	free(current);
}

t_token	*process_token(t_token **tokens, t_token *current,
	t_token_processor_ctx *ctx)
{
	if (current->type == TOKEN_WORD)
	{
		expand_token_value(current, ctx->exit_status, ctx->envp);
		if (current->value && current->value[0] == '\0')
		{
			handle_empty_token(tokens, current, ctx->prev, ctx->next);
			return (ctx->next);
		}
	}
	return (current);
}
