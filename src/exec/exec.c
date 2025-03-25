/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/25 22:11:49 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	search_binary_tree(t_vars *vars, t_nodes *cmds, bool pipe_in, bool pipe_out);

static void	recursive_call(t_vars *vars, t_nodes *cmds, bool is_pipe[2], bool call_left)
{
	if (call_left)
	{
		if (cmds->operator_type == TOKEN_PIPE)
			search_binary_tree(vars, cmds->left, false, true);
		else if (cmds->operator_type == TOKEN_OR)
			search_binary_tree(vars, cmds->left, is_pipe[0], is_pipe[1]);
		else
			search_binary_tree(vars, cmds->left, is_pipe[0], false);
	}
	else
	{
		if (cmds->operator_type == TOKEN_PIPE)
			search_binary_tree(vars, cmds->right, true, is_pipe[1]);
		else if (cmds->operator_type == TOKEN_OR)
			search_binary_tree(vars, cmds->right, is_pipe[0], is_pipe[1]);
		else
			search_binary_tree(vars, cmds->right, false, is_pipe[1]);
	}
}

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

static int	search_binary_tree(t_vars *vars, t_nodes *cmds, bool pipe_in, bool pipe_out)
{
	int	status;

	if (cmds && cmds->is_operator)
	{
		recursive_call(vars, cmds, (bool[2]){pipe_in, pipe_out}, true);
		if (cmds->operator_type != TOKEN_PIPE)
		{
			waitpid(vars->cmd.last_pid, &status, 0);
			if (cmds->operator_type == TOKEN_OR)
			{
				vars->cmd.last_exit_status = WEXITSTATUS(status);
				if (!vars->cmd.last_exit_status == 0)
					return (0);
			}
		}
		recursive_call(vars, cmds, (bool[2]){pipe_in, pipe_out}, false);
	}
	else if (cmds)
		exec_routine(vars, cmds, (bool[2]){pipe_in, pipe_out});
	return (0);
}

int	execute(t_vars *vars, t_nodes *cmds)
{
	init_pipes(vars->cmd.pipes);
	vars->cmd.pipes_count = 0;
	printf("%p\n", cmds);
	search_binary_tree(vars, cmds, false, false);
	close_pipe(vars->cmd.pipes, 3);
	free_branch(cmds, NULL);
	return (0);
}
