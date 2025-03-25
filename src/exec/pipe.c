/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:44 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/25 01:56:43 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(int pipes[2][2], int to_close)
{
	if (to_close == 1)
	{
		if (pipes[0][0] > 0)
			close(pipes[0][0]);
		pipes[0][0] = 0;
		if (pipes[0][1] > 0)
			close(pipes[0][1]);
		pipes[0][1] = 0;
	}
	else if (to_close == 2)
	{
		if (pipes[1][0] > 0)
			close(pipes[1][0]);
		pipes[1][0] = 0;
		if (pipes[1][1] > 0)
			close(pipes[1][1]);
		pipes[1][1] = 0;
	}
	else if (to_close == 3)
	{
		close_pipe(pipes, 1);
		close_pipe(pipes, 2);
	}
}

void	open_fd(t_redir *files, int *fd, bool is_heredoc, t_redir *tmp_node)
{
	char	*heredoc_path;

	while (files)
	{
		if (is_heredoc)
		{
			heredoc_path = get_tmp();//fichier tmp pour le heredoc
			*fd_in = open(heredoc_path, O_CREAT, 700);
			here_doc(*fd_in, files->filename);
			unlink(heredoc_path);
			free(heredoc_path);
		}
		else if (files->append)
			*fd = open(files->filename, O_APPEND | O_CREAT, 0777);
		else
			*fd = open(files->filename, O_TRUNC | O_CREAT, 0777);
		if (*fd == -1)
			perror(files->filename);
		tmp_node = files->next;
		free(files->filename);
		free(files);
		files = tmp_node;
		if (files && *fd != -1)
			close(*fd);
	}
}

static void	get_fd_in(t_vars *vars, t_nodes **cmds, bool is_pipe[2], int *fd_in)
{
	char	*heredoc_path;

	*fd_in = 0;
	if ((*cmds)->file_in)
		open_fd((*cmds)->file_in, fd_in, false, NULL);
	else if ((*cmds)->heredoc)
		open_fd((*cmds)->heredoc, fd_in, true, NULL);
	else if (is_pipe[0] == true)
		fd_in = &vars->cmd.pipes[vars->cmd.pipes_count % 2][0];
}

static void	get_fd_out(t_vars *vars, t_nodes **cmds, bool is_pipe[2], int *fd_out)
{
	*fd_out = 1;
	if ((*cmds)->file_out)
		open_fd((*cmds)->file_out, fd_out, false, NULL);
	else if (is_pipe[1] == true)
	{
		++vars->cmd.pipes_count;
		pipe(vars->cmd.pipes[vars->cmd.pipes_count % 2]);
		fd_out = &vars->cmd.pipes[vars->cmd.pipes_count % 2][1];
	}
}

int	exec_routine(t_vars *vars, t_nodes **cmds, bool is_pipe[2])
{
	int	pid;

	get_fd_in(vars, cmds, is_pipe, vars->cmd.fd_in);//fd_in priorite au fichier specifie puis here_doc puis pipe
	get_fd_out(vars, cmds, is_pipe, vars->cmd.fd_out);//fd_out priorite au fichier specifie puis pipe
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exec_cmd(vars, *cmds, pipes);
	return (0);
}
