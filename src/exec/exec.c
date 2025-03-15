/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/15 22:46:20 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	first_fd_in(t_vars *vars, t_nodes **cmds)
{
	int	fd_in;
	int	pipe_heredoc[2];

	fd_in = 0;
	if (*cmds->fd_in != -1)
		fd_in = *cmds->fd_in;
	else if (*cmds->here_doc == 1)
	{
		pipe(pipe_heredoc);
		here_doc(pipe_heredoc[1], cmds->delimiter);
		close(pipe_heredoc[1]);
		fd_in = pipe_heredoc[0];
	}
	return (fd_in);
}

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

int	execute(t_vars *vars, t_nodes **cmds)
{
	int	pipes[2][2];

	init_pipes(pipes);
	vars->cmd.pipes_count = 0;
	vars->cmd.fd_in = first_fd_in(vars, cmds);
	while (*cmds)
	{
		exec_routine(vars, cmds, pipes);
		if (*cmds->last_exit_status != 0 && *cmds->right)//si la commande est la pre;iere d'un || et fail
			*cmds = *cmds->right;
		else
			*cmds = *cmds->left;
	}
	close_pipes(pipes, 3);
}
