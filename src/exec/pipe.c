/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:44 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/16 20:20:27 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(int pipes[2][2], int to_close)
{
	if (to_close == 1)
	{
		if (pipes[1][0] > 0)
			close(pipes[0][0]);
		pipes[0][0] = 0;
		if (pipes[1][0] > 0)
			close(pipes[0][1]);
		pipes[0][1] = 0;
	}
	else if (to_close == 2)
	{
		if (pipes[1][0] > 0)
			close(pipes[1][0]);
		pipes[1][0] = 0;
		if (pipes[1][0] > 0)
			close(pipes[1][1]);
		pipes[1][1] = 0;
	}
	else if (to_close == 3)
	{
		close_pipe(pipes, 1);
		close_pipe(pipes, 2);
	}
}

static int	open_input_file(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (-2);
	}
	return (fd);
}

static int	handle_heredoc(t_redir *heredoc)
{
	int		fd;
	char	*heredoc_path;

	heredoc_path = get_tmp();
	if (!heredoc_path)
		return (-2);
	fd = open(heredoc_path, O_CREAT | O_WRONLY | O_TRUNC, 0700);
	if (fd == -1)
	{
		free(heredoc_path);
		return (-2);
	}
	here_doc(fd, heredoc->filename);
	close(fd);
	fd = open(heredoc_path, O_RDONLY);
	unlink(heredoc_path);
	free(heredoc_path);
	return (fd);
}

static int	get_fd_in(t_vars *vars, t_nodes **cmds, int pipes[2][2])
{
	int		fd_in;
	t_redir	*redir;

	fd_in = 0;
	if ((*cmds)->left->file_in)
	{
		redir = (*cmds)->left->file_in;
		while (redir->next)
			redir = redir->next;
		fd_in = open_input_file(redir->filename);
	}
	else if ((*cmds)->left->heredoc)
	{
		redir = (*cmds)->left->heredoc;
		while (redir->next)
			redir = redir->next;
		fd_in = handle_heredoc(redir);
	}
	else if ((*cmds)->is_operator && (*cmds)->operator_type == TOKEN_PIPE)
	{
		if (vars->cmd.pipes_count % 2 == 0)
			fd_in = pipes[0][0];
		if (vars->cmd.pipes_count % 2 == 1)
			fd_in = pipes[1][0];
	}
	return (fd_in);
}

static int	open_output_file(char *filename, int append)
{
	int fd;
	int flags;

	flags = O_WRONLY | O_CREAT;
	flags |= append ? O_APPEND : O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (-2);
	}
	return (fd);
}

static int	get_fd_out(t_vars *vars, t_nodes **cmds, int pipes[2][2])
{
	int		fd_out;
	t_redir	*redir;

	fd_out = 1;
	if ((*cmds)->file_out)
	{
		redir = (*cmds)->file_out;
		while (redir->next)
			redir = redir->next;
		fd_out = open_output_file(redir->filename, redir->append);
	}
	else if ((*cmds)->is_operator && (*cmds)->operator_type == TOKEN_PIPE)
	{
		++vars->cmd.pipes_count;
		if (vars->cmd.pipes_count % 2 == 0)
		{
			pipe(pipes[0]);
			fd_out = pipes[0][1];
		}
		if (vars->cmd.pipes_count % 2 == 1)
		{
			pipe(pipes[1]);
			fd_out = pipes[1][1];
		}
	}
	return (fd_out);
}

int	exec_routine(t_vars *vars, t_nodes **cmds, int pipes[2][2])
{
	int	pid;
	int	status;

	vars->cmd.fd_out = get_fd_out(vars, cmds, pipes);//fd_out priorite au fichier specifie puis pipe
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exec_cmd(vars, *cmds, pipes);
	if (((*cmds)->is_operator && (*cmds)->operator_type == TOKEN_AND) || *cmds->right)//si c'est un && j'attends le process aue je viens de lancer, si c'est un || j'attends pour voir si ca fail
	{
		waitpid(pid, &status, 0);
		vars->cmd.last_exit_status = WEXITSTATUS(status);
	}
	vars->cmd.fd_in = get_fd_in(vars, cmds, pipes);//fd_in priorite au fichier specifie puis here_doc puis pipe
	if (vars->cmd.fd_in == -2)
		return (-1);
	return (0);
}
