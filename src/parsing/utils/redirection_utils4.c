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
#include "wildcard_expander.h"
#include "redirection_utils.h"

static char	*handle_wildcard_expansion(char *expanded, char *filename)
{
	char	*final;
	char	**matches;
	int		count;

	count = count_matching_entries(expanded);
	if (count <= 0)
	{
		free(expanded);
		return (ft_strdup(filename));
	}
	matches = collect_matching_entries(expanded, count);
	if (!matches || !matches[0])
	{
		free(expanded);
		if (matches)
			free_array(matches);
		return (ft_strdup(filename));
	}
	final = ft_strdup(matches[0]);
	free(expanded);
	free_array(matches);
	return (final);
}

char	*expand_filename(char *filename, int exit_status, char **envp)
{
	char	*expanded;
	char	*processed;
	char	*final;

	processed = process_quotes_wrapper(filename);
	if (!processed)
		return (NULL);
	expanded = expand_variables(processed, exit_status, envp);
	free(processed);
	if (!expanded)
		return (NULL);
	if (has_unquoted_wildcard(expanded))
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
