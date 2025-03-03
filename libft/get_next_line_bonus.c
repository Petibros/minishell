/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:49:29 by sacgarci          #+#    #+#             */
/*   Updated: 2025/01/08 03:11:35 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	join_and_free(char **rest, char *buffer)
{
	char	*tmp;

	tmp = ft_calloc_gnl(1 + ft_strnlen(*rest, '\0'), sizeof(char));
	if (!tmp)
	{
		free(buffer);
		buffer = NULL;
		free(*rest);
		free(rest);
		return (0);
	}
	ft_strcpy(tmp, *rest);
	free(*rest);
	*rest = ft_strjoin_gnl(tmp, buffer);
	if (!*rest)
	{
		free(buffer);
		buffer = NULL;
		free(*rest);
		free(rest);
		return (0);
	}
	free(tmp);
	return (1);
}

static char	*get_line(char *rest, char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!rest[0])
		return (NULL);
	line = ft_calloc_gnl((2 + ft_strnlen(rest, '\n')), sizeof(char));
	if (!line)
		return (NULL);
	while (rest[i] && rest[i] != '\n')
	{
		line[i] = rest[i];
		i++;
	}
	if (rest[i] == '\n')
		line[i] = '\n';
	i = ft_strnlen(buffer, '\n');
	if (buffer[i] == '\n')
		i++;
	ft_strcpy(buffer, &buffer[i]);
	return (line);
}

static int	read_line(char *buffer, char **rest, int fd)
{
	int	has_read;

	has_read = 1;
	while (has_read)
	{
		if (!join_and_free(rest, buffer))
			return (0);
		if (ft_strchr_gnl(buffer, '\n'))
			break ;
		has_read = read(fd, buffer, BUFFER_SIZE);
		if (has_read == -1)
			return (0);
		buffer[has_read] = '\0';
	}
	return (1);
}

static char	*initialize(char *buffer, int fd)
{
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	if (!buffer)
		buffer = ft_calloc_gnl(1 + BUFFER_SIZE, sizeof(char));
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[4096];
	char		**rest;
	char		*line;

	line = NULL;
	buffer[fd] = initialize(buffer[fd], fd);
	if (!buffer[fd])
		return (NULL);
	rest = malloc(1 * sizeof(char *));
	*rest = ft_calloc_gnl(1, sizeof(char));
	if (!*rest)
		return (NULL);
	if (read_line(buffer[fd], rest, fd))
		line = get_line(*rest, buffer[fd]);
	free(*rest);
	free(rest);
	if (line)
		return (line);
	free(buffer[fd]);
	buffer[fd] = NULL;
	return (NULL);
}
