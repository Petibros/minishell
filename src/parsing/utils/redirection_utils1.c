/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:04:44 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 04:04:44 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "redirection_utils.h"
#include <fcntl.h>

t_redir	*create_redir_node(char *filename, int append)
{
	t_redir	*new;

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
	new->quoted = 0;
	new->next = NULL;
	return (new);
}

void	free_redir_list(t_redir *head)
{
	t_redir	*current;
	t_redir	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->filename);
		free(current);
		current = next;
	}
}
