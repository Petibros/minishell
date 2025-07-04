/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils2.c                               :+:      :+:    :+:   */
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

int	handle_input_redirection(t_nodes *node, char *filename)
{
	t_redir	*new;
	t_redir	*current;

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

int	handle_output_redirection(t_nodes *node, char *filename, int append)
{
	t_redir	*new;
	t_redir	*current;

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

static void	set_next(t_redir *new, t_nodes *node)
{
	t_redir	*current;

	if (!node->heredoc)
		node->heredoc = new;
	else
	{
		current = node->heredoc;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

int	handle_heredoc(t_nodes *node, char *delimiter)
{
	t_redir	*new;

	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
	{
		new = create_redir_node(delimiter, 0);
		if (!new)
			return (0);
		new->quoted = 1;
	}
	else
	{
		new = create_redir_node(delimiter, 0);
		if (!new)
			return (0);
		new->quoted = 0;
	}
	set_next(new, node);
	return (1);
}
