/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*post_process_expansion(char *result, char *str)
{
	char	*start;
	int		in_squote;

	if (!result || !ft_strchr(str, '$'))
		return (result);
	find_next_dollar(str, &in_squote);
	if (in_squote)
		return (result);
	start = ft_strchr(str, '$');
	return (process_variable(result, start));
}

char	*expand_variables(char *str, int exit_status, char **envp)
{
	t_quote_ctx	*ctx;
	char		*result;

	ctx = init_quote_context(str, exit_status, envp);
	if (!ctx)
		return (NULL);
	while (str[*(ctx->i)] && ctx->result)
		process_char(ctx);
	result = ctx->result;
	if (result && result[0] == '\0')
	{
		free(result);
		result = NULL;
	}
	free(ctx->i);
	free(ctx);
	return (post_process_expansion(result, str));
}
