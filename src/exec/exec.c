/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/18 01:38:03 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	search_binary_tree(t_vars *vars, t_nodes *cmds,
				int pipe_in, int pipe_out);

static int	is_next_to_subshell(t_nodes *cmds)
{
	if (cmds && ((cmds->is_operator && cmds->operator_type == TOKEN_SUBSHELL)
			|| (cmds->right && cmds->right->is_operator
				&& cmds->right->operator_type == TOKEN_SUBSHELL)))
		return (1);
	return (0);
}

static int	recursive_call(t_vars *vars, t_nodes *cmds,
		int is_pipe[2], bool call_left)
{
	int	res;

	if (call_left)
	{
		if (cmds->operator_type == TOKEN_PIPE)
			res = search_binary_tree(vars, cmds->left, 0, 1);
		else
			res = search_binary_tree(vars, cmds->left, is_pipe[0], 0);
	}
	else
	{
		if (cmds->operator_type == TOKEN_PIPE)
		{
			if (is_next_to_subshell(cmds->left))
				res = search_binary_tree(vars, cmds->right, 2, is_pipe[1]);
			else
				res = search_binary_tree(vars, cmds->right, 1, is_pipe[1]);
		}
		else
			res = search_binary_tree(vars, cmds->right, 0, is_pipe[1]);
	}
	return (res);
}

static void	init_pipes(int pipes[2][2], int pipes_subshell[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
	pipes_subshell[0][0] = 0;
	pipes_subshell[0][1] = 0;
	pipes_subshell[1][0] = 0;
	pipes_subshell[1][1] = 0;
}

static int	search_binary_tree(t_vars *vars, t_nodes *cmds,
		int pipe_in, int pipe_out)
{
	int	status;
	int	res;
	int	pid;

	res = 0;
	pid = 1;
	if (g_signal_received == SIGINT)
		return (130);
	if (cmds && cmds->is_operator)
	{
		if (cmds->operator_type == TOKEN_SUBSHELL)
		{
			if (pipe_out == 1)
			{
				++vars->cmd.pipes_count_sub;
				if (vars->cmd.pipes_count_sub > 2)
					close_pipe(vars->cmd.pipes_subshell, (vars->cmd.pipes_count_sub % 2) + 1);
				pipe(vars->cmd.pipes_subshell[vars->cmd.pipes_count_sub % 2]);
				pipe_out = 2;
			}
			pid = fork();
			if (pid == -1)
				return (-1);
			else if (pid == 0)
			{
				setup_signals_subshell();
				res = search_binary_tree(vars, cmds->left, pipe_in, pipe_out);
				status = wait_processes(vars->cmd.last_exit_status, vars->cmd.last_pid);
				close_pipe(vars->cmd.pipes, 3);
				close_pipe(vars->cmd.pipes_subshell, 3);
				free_all(vars, NULL, false);
				exit(status);
			}
			else if (pid > 0)
				return (0);
		}
		res = recursive_call(vars, cmds, (int[2]){pipe_in, pipe_out}, true);
		if (res == -1 || res == 130)
			return (res);
		if (cmds->operator_type != TOKEN_PIPE)
		{
			vars->cmd.last_exit_status = wait_processes
				(vars->cmd.last_exit_status, vars->cmd.last_pid);
			if (cmds->operator_type == TOKEN_OR)
			{
				if (vars->cmd.last_exit_status == 130
					|| vars->cmd.last_exit_status == 0)
					return (vars->cmd.last_exit_status);
			}
			else
			{
				if (vars->cmd.last_exit_status != 0)
					return (vars->cmd.last_exit_status);
			}
		}
		res = recursive_call(vars, cmds, (int[2]){pipe_in, pipe_out}, false);
		if (res == -1 || res == 130)
			return (res);
	}
	else if (cmds)
	{
		res = exec_routine(vars, cmds, (int[2]){pipe_in, pipe_out});
		close_fds(vars);
	}
	return (res);
}

int	wait_processes(int last_known_exit_status, int last_known_pid)
{
	int	status;
	int	pid;
	int	last_pid;
	int	last_status;

	last_pid = 0;
	last_status = -1;
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
	if (last_status == -1 || !last_known_pid)
		return (last_known_exit_status);
	return (WEXITSTATUS(last_status));
}

void	execute(t_vars *vars, t_nodes *cmds)
{
	int	status;

	init_pipes(vars->cmd.pipes, vars->cmd.pipes_subshell);
	vars->cmd.pipes_count = 0;
	vars->cmd.pipes_count_sub = 0;
	vars->cmd.last_pid = 0;
	status = search_binary_tree(vars, cmds, false, false);
	if (status == 130 || status == -1)
	{
		close_pipe(vars->cmd.pipes, 3);
		free_branch(vars->cmd.cmds, NULL);
		vars->cmd.cmds = NULL;
		g_signal_received = 0;
		return ;
	}
	vars->cmd.last_exit_status = wait_processes(vars->cmd.last_exit_status, vars->cmd.last_pid);
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
