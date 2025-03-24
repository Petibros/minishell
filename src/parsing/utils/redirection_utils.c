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

static t_redir	*create_redir_node(char *filename, int append)
{
	t_redir *new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->filename = ft_strdup(filename);
	if (!new->filename)
	{
		free(new);
		return (NULL);
	}
	new->append = append;
	new->next = NULL;
	return (new);
}

void	free_redir_list(t_redir *head)
{
	t_redir *current;
	t_redir *next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->filename);
		free(current);
		current = next;
	}
}

static int	handle_input_redirection(t_nodes *node, char *filename)
{
	t_redir *new;
	t_redir *current;

	new = create_redir_node(filename, 0);
	if (!new)
		return (0);
	if (!node->file_in)
		node->file_in = new;
	else
	{
		current = node->file_in;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (1);
}

static int	handle_output_redirection(t_nodes *node, char *filename, int append)
{
	t_redir *new;
	t_redir *current;

	new = create_redir_node(filename, append);
	if (!new)
		return (0);
	if (!node->file_out)
		node->file_out = new;
	else
	{
		current = node->file_out;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	return (1);
}

static int	handle_heredoc(t_nodes *node, char *delimiter)
{
	t_redir *new;
	t_redir *current;

	new = create_redir_node(delimiter, 0);
	if (!new)
		return (0);
	if (!node->heredoc)
		node->heredoc = new;
	else
	{
		current = node->heredoc;
		while (current->next)
			current = current->next;
		current->next = new;
	}
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
