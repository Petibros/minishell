/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:09:05 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 04:09:05 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "redirection_utils.h"
#include "quote_utils3.h"

char	*process_quotes_wrapper(char *filename)
{
	char	*processed;
	int		j;

	processed = malloc(sizeof(char) * (ft_strlen(filename) + 1));
	if (!processed)
		return (NULL);
	process_quotes(filename, processed, &j);
	processed[j] = '\0';
	return (processed);
}

int	validate_and_get_token(t_token **token, char **filename)
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

int	handle_redirection_type(t_nodes *node, t_token **token,
		char **envp, t_token_type type)
{
	char	*filename;
	char	*expanded_filename;
	int		result;

	if (!validate_and_get_token(token, &filename))
		return (0);
	if (type == TOKEN_HEREDOC)
	{
		expanded_filename = process_quotes_wrapper(filename);
		if (!expanded_filename)
			return (0);
	}
	else
	{
		expanded_filename = expand_filename(filename, 0, envp, type);
		if (!expanded_filename)
			return (0);
	}
	result = process_redirection(node, expanded_filename, type);
	free(expanded_filename);
	return (result);
}

int	process_redirection(t_nodes *node, char *expanded_filename,
		t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (handle_input_redirection(node, expanded_filename));
	else if (type == TOKEN_REDIR_OUT)
		return (handle_output_redirection(node, expanded_filename, 0));
	else if (type == TOKEN_APPEND)
		return (handle_output_redirection(node, expanded_filename, 1));
	else if (type == TOKEN_HEREDOC)
		return (handle_heredoc(node, expanded_filename));
	return (0);
}
