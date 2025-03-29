/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/28 00:59:06 by sacha            ###   ########.fr       */
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
			if (cmds->operator_type == TOKEN_OR)
			{
				waitpid(vars->cmd.last_pid, &status, 0);
				vars->cmd.last_exit_status = WEXITSTATUS(status);
				if (vars->cmd.last_exit_status == 0)
					return (0);
			}
			else
				vars->cmd.last_exit_status = wait_processes();
		}
		recursive_call(vars, cmds, (bool[2]){pipe_in, pipe_out}, false);
	}
	else if (cmds)
		exec_routine(vars, cmds, (bool[2]){pipe_in, pipe_out});
	return (0);
}

int	wait_processes(void)
{
	int	status;
	int	pid;
	int	last_pid;
	int	last_status;

	last_pid = 0;
	last_status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid != -1)
	{
		if (pid > last_pid)
		{
			last_pid = pid;
			last_status = status;
		}
		pid = waitpid(-1, &status, 0);
	}
	if (!last_status)
		return (0);
	return (WEXITSTATUS(last_status));
}

int	execute(t_vars *vars, t_nodes *cmds)
{
	init_pipes(vars->cmd.pipes);
	vars->cmd.pipes_count = 0;
	search_binary_tree(vars, cmds, false, false);
	vars->cmd.last_exit_status = wait_processes();
	close_pipe(vars->cmd.pipes, 3);
	free_branch(vars->cmd.cmds, NULL);
	return (0);
}
