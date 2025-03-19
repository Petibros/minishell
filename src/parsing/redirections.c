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
#include <fcntl.h>

static int	handle_input_redirection(t_nodes *node, char *filename)
{
	int	fd;

	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	node->fd_in = fd;
	return (1);
}

static int	handle_output_redirection(t_nodes *node, char *filename, int append)
{
	int	fd;
	int	flags;

	if (node->fd_out != STDOUT_FILENO)
		close(node->fd_out);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	node->fd_out = fd;
	node->append_out = append;
	return (1);
}

static int	handle_heredoc(t_nodes *node, char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (0);
	node->here_doc = 1;
	node->delimiter = ft_strdup(delimiter);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strlen(line) - 1 == ft_strlen(delimiter) &&
			!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	if (node->fd_in != STDIN_FILENO)
		close(node->fd_in);
	node->fd_in = pipe_fd[0];
	return (1);
}

int	handle_redirections(t_nodes *node, t_token **token)
{
	t_token_type	type;
	char			*filename;

	while (*token && ((*token)->type == TOKEN_REDIR_IN ||
			(*token)->type == TOKEN_REDIR_OUT ||
			(*token)->type == TOKEN_APPEND ||
			(*token)->type == TOKEN_HEREDOC))
	{
		type = (*token)->type;
		*token = (*token)->next;
		if (!*token || (*token)->type != TOKEN_WORD)
			return (0);
		filename = (*token)->value;
		*token = (*token)->next;
		if (type == TOKEN_REDIR_IN)
		{
			if (!handle_input_redirection(node, filename))
				return (0);
		}
		else if (type == TOKEN_REDIR_OUT)
		{
			if (!handle_output_redirection(node, filename, 0))
				return (0);
		}
		else if (type == TOKEN_APPEND)
		{
			if (!handle_output_redirection(node, filename, 1))
				return (0);
		}
		else if (type == TOKEN_HEREDOC)
		{
			if (!handle_heredoc(node, filename))
				return (0);
		}
	}
	return (1);
}
