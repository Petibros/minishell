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
#include "quote_utils.h"

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

int	handle_redirection_type(t_nodes *node, t_token **token, t_token_type type)
{
	char	*filename;
	int		result;

	if (!validate_and_get_token(token, &filename))
		return (0);
	if (type == TOKEN_HEREDOC)
	{
		result = handle_heredoc(node, filename);
		return (result);
	}
	result = process_redirection(node, filename, type);
	return (result);
}

int	process_redirection(t_nodes *node, char *filename,
		t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (handle_input_redirection(node, filename));
	else if (type == TOKEN_REDIR_OUT)
		return (handle_output_redirection(node, filename, 0));
	else if (type == TOKEN_APPEND)
		return (handle_output_redirection(node, filename, 1));
	else if (type == TOKEN_HEREDOC)
		return (handle_heredoc(node, filename));
	return (0);
}
