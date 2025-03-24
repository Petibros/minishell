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
