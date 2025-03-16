/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/16 20:28:39 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	first_fd_in(t_vars *vars, t_nodes **cmds)
{
	int		fd_in;
	char	*heredoc_path;

	fd_in = 0;
	if (*cmds->fd_in != -1)
		fd_in = *cmds->fd_in;
	else if (*cmds->here_doc == 1)
	{
		heredoc_path = get_tmp();
		if (!heredoc_path)
			return (-2);
		fd_in = open(heredoc_path, O_CREAT, 700);
		here_doc(fd_in, cmds->delimiter);
		unlink(heredoc_path);
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
	if (vars->cmd.fd_in == -1)
		return (-1);
	while (*cmds)
	{
		if (exec_routine(vars, cmds, pipes) == -1)
			return (-1);
		if (*cmds->last_exit_status != 0 && *cmds->right)//si la commande est la pre;iere d'un || et fail
			*cmds = *cmds->right;
		else
			*cmds = *cmds->left;
	}
	close_pipes(pipes, 3);
	return (0);
}
