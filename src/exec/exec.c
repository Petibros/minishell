/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/06 21:02:35 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	search_binary_tree(t_vars *vars, t_nodes *cmds,
				bool pipe_in, bool pipe_out);

static int	recursive_call(t_vars *vars, t_nodes *cmds,
		bool is_pipe[2], bool call_left)
{
	int	res;

	if (call_left)
	{
		if (cmds->operator_type == TOKEN_PIPE)
			res = search_binary_tree(vars, cmds->left, false, true);
		else if (cmds->operator_type == TOKEN_OR)
			res = search_binary_tree(vars, cmds->left, is_pipe[0], is_pipe[1]);
		else
			res = search_binary_tree(vars, cmds->left, is_pipe[0], false);
	}
	else
	{
		if (cmds->operator_type == TOKEN_PIPE)
			res = search_binary_tree(vars, cmds->right, true, is_pipe[1]);
		else if (cmds->operator_type == TOKEN_OR)
			res = search_binary_tree(vars, cmds->right, is_pipe[0], is_pipe[1]);
		else
			res = search_binary_tree(vars, cmds->right, false, is_pipe[1]);
	}
	return (res);
}

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

static int	search_binary_tree(t_vars *vars, t_nodes *cmds,
		bool pipe_in, bool pipe_out)
{
	int	status;
	int	res;

	res = 0;
	if (g_signal_received == SIGINT)
		return (130);
	if (cmds && cmds->is_operator)
	{
		res = recursive_call(vars, cmds, (bool[2]){pipe_in, pipe_out}, true);
		if (res == -1 || res == 130)
			return (res);
		if (cmds->operator_type != TOKEN_PIPE)
		{
			if (cmds->operator_type == TOKEN_OR)
			{
				waitpid(vars->cmd.last_pid, &status, 0);
				if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
				{
					vars->cmd.last_exit_status = 130;
					return (130);
				}
				vars->cmd.last_exit_status = WEXITSTATUS(status);
				if (vars->cmd.last_exit_status == 0)
					return (0);
			}
			else
			{
				vars->cmd.last_exit_status
					= wait_processes(vars->cmd.last_exit_status);
				if (vars->cmd.last_exit_status == 130)
					return (130);
			}
		}
		res = recursive_call(vars, cmds, (bool[2]){pipe_in, pipe_out}, false);
		if (res == -1 || res == 130)
			return (res);
	}
	else if (cmds)
		res = exec_routine(vars, cmds, (bool[2]){pipe_in, pipe_out});
	return (res);
}

int	wait_processes(int last_known_exit_status)
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
	if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGINT)
	{
		g_signal_received = SIGINT;
		return (130);
	}
	if (!last_status)
		return (last_known_exit_status);
	return (WEXITSTATUS(last_status));
}

void	execute(t_vars *vars, t_nodes *cmds)
{
	int	status;

	init_pipes(vars->cmd.pipes);
	vars->cmd.pipes_count = 0;
	status = search_binary_tree(vars, cmds, false, false);
	if (status == 130 || status == -1)
	{
		close_pipe(vars->cmd.pipes, 3);
		free_branch(vars->cmd.cmds, NULL);
		vars->cmd.cmds = NULL;
		g_signal_received = 0;
		return ;
	}
	vars->cmd.last_exit_status = wait_processes(vars->cmd.last_exit_status);
	if (vars->cmd.last_exit_status == 130)
	{
		close_pipe(vars->cmd.pipes, 3);
		free_branch(vars->cmd.cmds, NULL);
		vars->cmd.cmds = NULL;
		g_signal_received = 0;
		return ;
	}
	close_pipe(vars->cmd.pipes, 3);
	free_branch(vars->cmd.cmds, NULL);
	vars->cmd.cmds = NULL;
}
