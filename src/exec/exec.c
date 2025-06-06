/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/06/02 16:01:20 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	search_binary_tree(t_vars *vars, t_nodes *cmds,
				int pipe_in, int pipe_out);

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
			res = search_binary_tree(vars, cmds->right, 1, is_pipe[1]);
		}
		else
			res = search_binary_tree(vars, cmds->right, 0, is_pipe[1]);
	}
	return (res);
}

int	create_subshell(t_vars *vars, t_nodes *cmds,
	int pipe_in, int pipe_out)
{
	int	pid;
	int	code;

	code = get_fds(vars, cmds, (int [2]){pipe_in, pipe_out});
	if (code != 0)
		return (code);
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		dup2(vars->cmd.fd_in, 0);
		dup2(vars->cmd.fd_out, 1);
		close_child_fds(vars, vars->cmd.pipes);
		setup_signals_subshell();
		vars->sa_setup = &setup_signals_subshell;
		search_binary_tree(vars, cmds->left, 0, 0);
		code = wait_processes(vars->cmd.last_exit_status, vars->cmd.last_pid);
		close_pipe(vars->cmd.pipes, 3);
		free_all(vars, NULL, false);
		rl_clear_history();
		exit(code);
	}
	vars->cmd.last_pid = pid;
	return (0);
}

static int	operator_routine(t_vars *vars, t_nodes *cmds)
{
	if (cmds->operator_type != TOKEN_PIPE)
	{
		vars->cmd.last_exit_status = wait_processes
			(vars->cmd.last_exit_status, vars->cmd.last_pid);
		if (cmds->operator_type == TOKEN_OR)
		{
			if (vars->cmd.last_exit_status == 130
				|| vars->cmd.last_exit_status == 0
				|| vars->cmd.last_exit_status == 131)
				return (1);
		}
		else
		{
			if (vars->cmd.last_exit_status != 0)
				return (1);
		}
	}
	return (0);
}

static int	search_binary_tree(t_vars *vars, t_nodes *cmds,
		int pipe_in, int pipe_out)
{
	int	res;

	res = 0;
	if (g_signal_received == SIGINT)
		return (130);
	if (cmds && cmds->is_operator)
	{
		if (cmds->operator_type == TOKEN_SUBSHELL)
			return (subshell_and_clean(vars, cmds, pipe_in, pipe_out));
		res = recursive_call(vars, cmds, (int [2]){pipe_in, pipe_out}, true);
		if (res == -1 || res == 130 || res == 131)
			return (res);
		res = operator_routine(vars, cmds);
		if (res != 0)
			return (vars->cmd.last_exit_status);
		res = recursive_call(vars, cmds, (int [2]){pipe_in, pipe_out}, false);
	}
	else if (cmds)
	{
		res = exec_routine(vars, cmds, (int [2]){pipe_in, pipe_out});
		close_fds(vars);
	}
	return (res);
}

void	execute(t_vars *vars, t_nodes *cmds)
{
	int	status;

	init_pipes(vars->cmd.pipes);
	vars->cmd.pipes_count = 0;
	status = search_binary_tree(vars, cmds, false, false);
	if (status != 131 && status != 130 && status != -1)
		vars->cmd.last_exit_status = wait_processes
			(vars->cmd.last_exit_status, vars->cmd.last_pid);
	close_pipe(vars->cmd.pipes, 3);
	free_branch(vars->cmd.cmds, NULL);
	vars->cmd.cmds = NULL;
	g_signal_received = 0;
}
