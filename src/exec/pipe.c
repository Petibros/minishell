/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:44 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/28 03:44:29 by sacha            ###   ########.fr       */
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

void	open_fd(t_redir **redirs, int *fd, int fd_type)
{
	char	*heredoc_path;
	t_redir	*files;

	files = *redirs;
	while (files)
	{
		if (fd_type == 2)
		{
			heredoc_path = get_tmp();//fichier tmp pour le heredoc
			*fd = open(heredoc_path, O_TRUNC | O_WRONLY | O_CREAT, 0777);
			here_doc(*fd, files->filename);
			close(*fd);
			*fd = open(heredoc_path, O_RDONLY);
			unlink(heredoc_path);
			free(heredoc_path);
		}
		else if (fd_type == 3 && files->append)
			*fd = open(files->filename, O_WRONLY | O_APPEND | O_CREAT, 0777);
		else if (fd_type == 3)
			*fd = open(files->filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		else if (fd_type == 1)
			*fd = open(files->filename, O_RDONLY);
		if (*fd == -1)
			perror(files->filename);
		files = files->next;
		if (files && *fd != -1)
			close(*fd);
	}
}

static void	get_fd_in(t_vars *vars, t_nodes *cmds, bool is_pipe[2], int *fd_in)
{
	*fd_in = 0;
	if (cmds->file_in)
		open_fd(&cmds->file_in, fd_in, 1);
	else if (cmds->heredoc)
		open_fd(&cmds->heredoc, fd_in, 2);
	else if (is_pipe[0] == true)
		*fd_in = vars->cmd.pipes[vars->cmd.pipes_count % 2][0];
}

static void	get_fd_out(t_vars *vars, t_nodes *cmds, bool is_pipe[2], int *fd_out)
{
	*fd_out = 1;
	if (cmds->file_out)
		open_fd(&cmds->file_out, fd_out, 3);
	else if (is_pipe[1] == true)
	{
		++vars->cmd.pipes_count;
		pipe(vars->cmd.pipes[vars->cmd.pipes_count % 2]);
		*fd_out = vars->cmd.pipes[vars->cmd.pipes_count % 2][1];
	}
}

int	exec_routine(t_vars *vars, t_nodes *cmds, bool is_pipe[2])
{
	int	pid;

	get_fd_in(vars, cmds, is_pipe, &vars->cmd.fd_in);//fd_in priorite au fichier specifie puis here_doc puis pipe
	get_fd_out(vars, cmds, is_pipe, &vars->cmd.fd_out);//fd_out priorite au fichier specifie puis pipe
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		exec_cmd(vars, cmds, vars->cmd.pipes);
	}
	close_fds(vars->cmd.pipes, vars);
	vars->cmd.last_pid = pid;
	return (0);
}
