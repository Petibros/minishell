/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/24 05:24:42 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	first_fd_in(t_nodes **cmds, int *fd_in)
{
	char	*heredoc_path;

	*fd_in = 0;
	if ((*cmds)->fd_in != -1)
		fd_in = &(*cmds)->fd_in;
	else if ((*cmds)->here_doc == 1)
	{
		heredoc_path = get_tmp();
		if (!heredoc_path)
		{
			fd_in = NULL;
			return ;
		}
		*fd_in = open(heredoc_path, O_CREAT, 700);
		here_doc(*fd_in, (*cmds)->delimiter);
		unlink(heredoc_path);
	}
}

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

static int search_binary_tree(t_vars *vars, t_nodes **cmds, int pipes[2][2])
{
	int	status;

	if ((*cmds)->is_operator)
	{
		search_binary_tree(vars, (*cmds)->left, pipes);
		if ((*cmds)->operator_type != TOKEN_PIPE)
		{
			waitpid(vars->cmd.last_pid, &status, 0);
			if ((*cmds)->operator_type == TOKEN_OR)
			{
				vars->cmd.last_exit_status = WEXITSTATUS(status);
				if (!vars->cmd.last_exit_status == 0)
					return (0);
			}
		}
		search_binary_tree(vars, (*cmds)->right, pipes);
	}
	else
		exec_cmd(vars, *cmds, pipes);
}

int	execute(t_vars *vars, t_nodes **cmds)
{
	int	pipes[2][2];

	init_pipes(pipes);
	vars->cmd.pipes_count = 0;
	search_binary_tree(vars, cmds);
	close_pipe(pipes, 3);
	return (0);
}
