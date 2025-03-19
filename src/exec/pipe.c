/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:44 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/19 23:57:03 by sacgarci         ###   ########.fr       */
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

static void	get_fd_in(t_vars *vars, t_nodes **cmds, int pipes[2][2], int *fd_in)
{
	char	*heredoc_path;

	*fd_in = 0;
	if ((*cmds)->left->fd_in != -2)
		fd_in = &(*cmds)->left->fd_in;
	else if ((*cmds)->left->here_doc == 1)
	{
		heredoc_path = get_tmp();//fichier tmp pour le heredoc
		if (!heredoc_path)
		{
			fd_in = NULL;
			return ;
		}
		*fd_in = open(heredoc_path, O_CREAT, 700);
		here_doc(*fd_in, (*cmds)->left->delimiter);
		unlink(heredoc_path);
		free(heredoc_path);
	}
	else if ((*cmds)->next_operator == PIPE)
	{
		if (vars->cmd.pipes_count % 2 == 0)
			fd_in = &pipes[0][0];
		if (vars->cmd.pipes_count % 2 == 1)
			fd_in = &pipes[1][0];
	}
}

static void	get_fd_out(t_vars *vars, t_nodes **cmds, int pipes[2][2], int *fd_out)
{
	*fd_out = 1;
	if ((*cmds)->fd_out != -2)
		fd_out = &(*cmds)->fd_out;
	else if ((*cmds)->next_operator == PIPE)
	{
		++vars->cmd.pipes_count;
		if (vars->cmd.pipes_count % 2 == 0)
		{
			pipe(pipes[0]);
			fd_out = &pipes[0][1];
		}
		if (vars->cmd.pipes_count % 2 == 1)
		{
			pipe(pipes[1]);
			fd_out = &pipes[1][1];
		}
	}
}

int	exec_routine(t_vars *vars, t_nodes **cmds, int pipes[2][2])
{
	int	pid;
	int	status;

	get_fd_out(vars, cmds, pipes, vars->cmd.fd_out);//fd_out priorite au fichier specifie puis pipe
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exec_cmd(vars, *cmds, pipes);
	if ((*cmds)->next_operator == AND || (*cmds)->right) //si c'est un && j'attends le process aue je viens de lancer, si c'est un || j'attends pour voir si ca fail
	{
		waitpid(pid, &status, 0);
		vars->cmd.last_exit_status = WEXITSTATUS(status);
	}
	get_fd_in(vars, cmds, pipes, vars->cmd.fd_in);//fd_in priorite au fichier specifie puis here_doc puis pipe
	if (vars->cmd.fd_in == NULL)
		return (-1);
	return (0);
}
