/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:09:05 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 04:09:05 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "redirection_utils.h"

static char	*handle_wildcard_expansion(char *expanded, char *filename)
{
	char	*final;

	if (!expanded)
		return (ft_strdup(filename));
	final = new_expand_wildcard(expanded);
	free(expanded);
	if (!final || final[0] == '\0')
	{
		free(final);
		return (ft_strdup(filename));
	}
	return (final);
}

char	*expand_filename(char *filename, int exit_status, char **envp,
		t_token_type type)
{
	char	*expanded;
	char	*processed;
	char	*final;
	t_vars	vars;

	processed = process_quotes_wrapper(filename);
	if (!processed)
		return (NULL);
	vars.cmd.last_exit_status = exit_status;
	expanded = new_get_expanded_str(processed, envp, &vars);
	free(processed);
	if (!expanded)
		return (NULL);
	if (ft_strchr(expanded, '*') && type != TOKEN_HEREDOC)
	{
		final = handle_wildcard_expansion(expanded, filename);
		return (final);
	}
	return (expanded);
}

int	handle_redirections(t_nodes *node, t_token **token, char **envp)
{
	t_token_type	type;
	int				result;

	while (*token && ((*token)->type == TOKEN_REDIR_IN
			|| (*token)->type == TOKEN_REDIR_OUT
			|| (*token)->type == TOKEN_APPEND
			|| (*token)->type == TOKEN_HEREDOC))
	{
		type = (*token)->type;
		*token = (*token)->next;
		result = handle_redirection_type(node, token, envp, type);
		if (!result)
			return (0);
	}
	return (1);
}
