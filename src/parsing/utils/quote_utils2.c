/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:46:35 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:46:35 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	handle_quotes_in_redir(t_redir *redir)
{
	char	*tmp;
	t_redir	*current;

	current = redir;
	while (current)
	{
		if (current->filename)
		{
			tmp = remove_quotes(current->filename);
			if (tmp)
			{
				free(current->filename);
				current->filename = tmp;
			}
		}
		current = current->next;
	}
}

static void	handle_quotes_in_argv(t_nodes *node)
{
	char	*tmp;
	int		i;

	i = 0;
	while (node->argv[i])
	{
		tmp = remove_quotes(node->argv[i]);
		if (tmp)
		{
			free(node->argv[i]);
			node->argv[i] = tmp;
		}
		i++;
	}
}

void	handle_quotes_in_node(t_nodes *node)
{
	if (!node || !node->argv)
		return ;
	handle_quotes_in_argv(node);
	if (node->file_in)
		handle_quotes_in_redir(node->file_in);
	if (node->file_out)
		handle_quotes_in_redir(node->file_out);
	if (node->heredoc)
		handle_quotes_in_redir(node->heredoc);
}
