/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:09:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/18 20:09:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "wildcard_expander.h"
#include <fcntl.h>

static int	handle_input_redirection(t_nodes *node, char *filename)
{
	if (node->file_in)
		free(node->file_in);
	node->file_in = ft_strdup(filename);
	if (!node->file_in)
		return (0);
	return (1);
}

static int	handle_output_redirection(t_nodes *node, char *filename, int append)
{
	if (node->file_out)
		free(node->file_out);
	node->file_out = ft_strdup(filename);
	if (!node->file_out)
		return (0);
	node->append_out = append;
	return (1);
}

static int	handle_heredoc(t_nodes *node, char *delimiter)
{
	node->here_doc = 1;
	if (node->delimiter)
		free(node->delimiter);
	node->delimiter = ft_strdup(delimiter);
	if (!node->delimiter)
		return (0);
	return (1);
}

static char *expand_filename(char *filename, int exit_status)
{
	char *expanded;
	char *final;
	char **matches;
	int count;

	// First expand variables
	expanded = expand_variables(filename, exit_status);
	if (!expanded)
		return (NULL);

	// Then expand wildcards if present
	if (has_unquoted_wildcard(expanded))
	{
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
		// Use only the first match for redirections
		final = ft_strdup(matches[0]);
		free(expanded);
		free_array(matches);
		return (final);
	}
	return (expanded);
}

int	handle_redirections(t_nodes *node, t_token **token)
{
	t_token_type	type;
	char			*filename;
	char			*expanded_filename;

	while (*token && ((*token)->type == TOKEN_REDIR_IN ||
			(*token)->type == TOKEN_REDIR_OUT ||
			(*token)->type == TOKEN_APPEND ||
			(*token)->type == TOKEN_HEREDOC))
	{
		type = (*token)->type;
		*token = (*token)->next;
		if (!*token || (*token)->type != TOKEN_WORD)
		{
			print_syntax_error(NULL); // Print newline error for missing filename
			return (0);
		}
		filename = (*token)->value;
		*token = (*token)->next;

		// Always expand filenames and heredoc delimiters
		expanded_filename = expand_filename(filename, 0); // TODO: Pass proper exit_status
		if (!expanded_filename)
			return (0);

		if (type == TOKEN_REDIR_IN)
		{
			if (!handle_input_redirection(node, expanded_filename))
			{
				free(expanded_filename);
				return (0);
			}
		}
		else if (type == TOKEN_REDIR_OUT)
		{
			if (!handle_output_redirection(node, expanded_filename, 0))
			{
				free(expanded_filename);
				return (0);
			}
		}
		else if (type == TOKEN_APPEND)
		{
			if (!handle_output_redirection(node, expanded_filename, 1))
			{
				free(expanded_filename);
				return (0);
			}
		}
		else if (type == TOKEN_HEREDOC)
		{
			if (!handle_heredoc(node, expanded_filename))
			{
				free(expanded_filename);
				return (0);
			}
		}
		free(expanded_filename);
	}
	return (1);
}
