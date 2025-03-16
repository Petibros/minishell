/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:30:13 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/16 21:23:43 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tmp(void)
{
	char	tmp_path[64];
	int		i;

	i = 14;
	ft_strlcpy(tmp_path, "/tmp/here_doc-0", 64);
	while (access(tmp_path, F_OK) == 0)
	{
		if (tmp_path[i] == 9)
		++tmp_path[i];
	}
}

void	here_doc(int fd, char *limiter)
{
	char	*buffer;

	buffer = readline("heredoc > ");
	while (ft_strncmp(limiter, buffer, ft_strlen(limiter) + 1))
	{
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		free(buffer);
		buffer = readline("heredoc > ");
	}
	free(buffer);
}
