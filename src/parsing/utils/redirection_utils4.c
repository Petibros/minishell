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

char	*expand_filename(char *filename, int exit_status)
{
	char	*expanded;

	expanded = expand_variables(filename, exit_status);
	if (!expanded)
		return (NULL);
	if (has_unquoted_wildcard(expanded))
		return (handle_wildcard_expansion(expanded, filename));
	return (expanded);
}

static int	validate_and_get_token(t_token **token, char **filename)
{
	if (!*token || (*token)->type != TOKEN_WORD)
	{
		print_syntax_error(NULL);
		return (0);
	}
	*filename = (*token)->value;
	*token = (*token)->next;
	return (1);
}

int	handle_redirections(t_nodes *node, t_token **token)
{
	t_token_type	type;
	char			*filename;
	char			*expanded_filename;
	int				result;

	while (*token && ((*token)->type == TOKEN_REDIR_IN
			|| (*token)->type == TOKEN_REDIR_OUT
			|| (*token)->type == TOKEN_APPEND
			|| (*token)->type == TOKEN_HEREDOC))
	{
		type = (*token)->type;
		*token = (*token)->next;
		if (!validate_and_get_token(token, &filename))
			return (0);
		expanded_filename = expand_filename(filename, 0);
		if (!expanded_filename)
			return (0);
		result = process_redirection(node, expanded_filename, type);
		free(expanded_filename);
		if (!result)
			return (0);
	}
	return (1);
}
