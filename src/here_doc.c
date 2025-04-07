/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:30:13 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/07 06:49:39 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tmp(void)
{
	char	tmp_path[32];
	char	*heredoc_path;
	int		i;

	ft_strlcpy(tmp_path, "/tmp/here_doc-00000000000000000", 32);
	while (access(tmp_path, F_OK) == 0)
	{
		i = 30;
		while (ft_isdigit(tmp_path[i]) && tmp_path[i] == 9)
		{
			tmp_path[i] = 0;
			--i;
		}
		if (ft_isdigit(tmp_path[i]) == 0)
		{
			write(2, "Not enough space for heredoc file\n", 34);
			return (NULL);
		}
		++tmp_path[i];
	}
	heredoc_path = ft_strdup(tmp_path);
	if (!heredoc_path)
		perror("heredoc_path: malloc error");
	return (heredoc_path);
}

void	heredoc_gestion(t_vars *vars, t_redir *files, int *fd)
{
	int		dup_fd;
	char	*heredoc_path;

	heredoc_path = get_tmp();//fichier tmp pour le heredoc
	if (!heredoc_path)
	{
		*fd = -2;
		return ;
	}
	*fd = open(heredoc_path, O_TRUNC | O_WRONLY | O_CREAT, 0777);
	signal(SIGINT, SIG_DFL);
	setup_signals_child();
	dup_fd = dup(0);
	here_doc(*fd, files->filename);
	if (g_signal_received == SIGINT)
		dup2(dup_fd, 0);
	close(dup_fd);
	close(*fd);
	*fd = open(heredoc_path, O_RDONLY);
	unlink(heredoc_path);
	free(heredoc_path);
	if (g_signal_received == SIGINT)
	{
		close(*fd);
		vars->cmd.last_exit_status = 130;
		*fd = -3;
	}
	signal(SIGINT, SIG_DFL);
	setup_signals();
}

int	here_doc(int fd, char *limiter)
{
	char	*buffer;

	buffer = readline("heredoc > ");
	if (g_signal_received == SIGINT)
	{
		free(buffer);
		return (130);
	}
	while (buffer && ft_strncmp(limiter, buffer, ft_strlen(limiter) + 1))
	{
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		free(buffer);
		buffer = readline("heredoc > ");
		if (g_signal_received == SIGINT)
		{
			free(buffer);
			return (130);
		}
	}
	if (!buffer)
	{
		write(2, "Warning: got end-of-file, expected: ", 36);
		write(2, limiter, ft_strlen(limiter));
		write(2, "\n", 1);
		return (0);
	}
	free(buffer);
	return (0);
}
