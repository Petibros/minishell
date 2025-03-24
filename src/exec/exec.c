/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/24 18:16:50 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	recursive_call()

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

static int	search_binary_tree(t_vars *vars, t_nodes **cmds, bool pipe_in, bool pipe_out)
{
	int	status;

	if (*cmds && (*cmds)->is_operator)
	{
		if ((*cmds)->operator_type == TOKEN_PIPE)
			search_binary_tree(vars, (*cmds)->left, false, true);
		else if ((*cmds)->operator_type == TOKEN_OR)
			search_binary_tree(vars, (*cmds)->left, pipe_in, pipe_out);
		else
			search_binary_tree(vars, (*cmds)->left, pipe_in, false);
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
		if ((*cmds)->operator_type == TOKEN_PIPE)
			search_binary_tree(vars, (*cmds)->right, true, pipe_out);
		else if ((*cmds)->operator_type == TOKEN_OR)
			search_binary_tree(vars, (*cmds)->left, pipe_in, pipe_out);
		else
			search_binary_tree(vars, (*cmds)->left, pipe_in, false);
	}
	else if (*cmds)
		exec_cmd(vars, *cmds);
}

int	execute(t_vars *vars, t_nodes **cmds)
{
	init_pipes(vars->cmd.pipes);
	vars->cmd.pipes_count = 0;
	search_binary_tree(vars, cmds, false, false);
	close_pipe(vars->cmd.pipes, 3);
	return (0);
}
