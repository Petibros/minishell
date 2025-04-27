/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils5.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 06:53:09 by npapash           #+#    #+#             */
/*   Updated: 2025/04/27 19:43:57 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

int	find_next_dollar(char *str, int *in_squote)
{
	int	i;

	i = 0;
	*in_squote = 0;
	while (str[i] && str[i] != '$')
	{
		if (str[i] == '\'')
			*in_squote = !*in_squote;
		i++;
	}
	return (i);
}

char	*process_variable(char *result, char *start)
{
	int		var_len;
	char	*var_in_result;

	if (!start || (!ft_isalpha(*(start + 1)) && *(start + 1) != '_'))
		return (result);
	var_len = get_var_length(start + 1);
	var_in_result = ft_strnstr(result, start, ft_strlen(result));
	if (!var_in_result)
		return (result);
	return (extract_variable_part(result, var_in_result, var_len));
}

void	process_char(t_quote_ctx *ctx)
{
	static int	in_squote = 0;

	if (ctx->str[ctx->i] == '\'')
	{
		if (!in_squote)
			in_squote = 1;
		ctx->result = handle_single_quote(ctx->str, &ctx->i, ctx->result);
		in_squote = 0;
	}
	else if (!in_squote && ctx->str[ctx->i] == '"')
		ctx->result = handle_double_quote_char(ctx);
	else if (!in_squote && ctx->str[ctx->i] == '$')
		ctx->result = handle_dollar_sign(ctx);
	else
	{
		if (in_squote)
			ctx->result = handle_in_squote(ctx);
		else
			ctx->result = handle_regular_char(ctx->str, &ctx->i, ctx->result);
	}
}
