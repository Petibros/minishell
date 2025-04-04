/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:30:13 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/04 23:52:29 by sacha            ###   ########.fr       */
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
		if (i != 30)
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
	int		pid;
	char	*heredoc_path;

	heredoc_path = get_tmp();//fichier tmp pour le heredoc
	if (!heredoc_path)
		return ;
	*fd = open(heredoc_path, O_TRUNC | O_WRONLY | O_CREAT, 0777);
	pid = fork();
	if (pid == -1)
	{
		perror("error during fork");
		free(heredoc_path);
		return ;
	}
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		here_doc(*fd, files->filename, heredoc_path);
		free(heredoc_path);
		free_all(vars, NULL, false);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	close(*fd);
	*fd = open(heredoc_path, O_RDONLY);
	unlink(heredoc_path);
	free(heredoc_path);
}

void	here_doc(int fd, char *limiter, char *heredoc_path)
{
	char	*buffer;

	buffer = readline("heredoc > ");
	while (buffer && ft_strncmp(limiter, buffer, ft_strlen(limiter) + 1))
	{
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		free(buffer);
		buffer = readline("heredoc > ");
	}
	if (!buffer)
	{
		write(2, "Warning: got end-of-file, expected: ", 36);
		write(2, limiter, ft_strlen(limiter));
		write(2, "\n", 1);
		close(fd);
		open(heredoc_path, O_TRUNC | O_WRONLY);
		return ;
	}
	free(buffer);
}
