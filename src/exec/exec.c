/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/24 19:43:27 by sacha            ###   ########.fr       */
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

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
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
			get_fd_in(vars, cmds, (int [2]){pipe_in, pipe_out},
				&vars->cmd.fd_in);
			get_fd_out(vars, cmds, (int [2]){pipe_in, pipe_out},
				&vars->cmd.fd_out);
			if (vars->cmd.fd_in <= -2)
			{
				close_fds(vars);
				if (vars->cmd.fd_in == -3)
					return (130);
				return (-1);
			}
			if (vars->cmd.fd_in == -1 || vars->cmd.fd_out == -1)
			{
				close_fds(vars);
				vars->cmd.last_exit_status = 0;
				if (vars->cmd.fd_in == -1 || vars->cmd.fd_out == -1)
					vars->cmd.last_exit_status = 1;
				return (0);
			}
			pid = fork();
			if (pid == -1)
				return (-1);
			else if (pid == 0)
			{
				dup2(vars->cmd.fd_in, 0);
				dup2(vars->cmd.fd_out, 1);
				close_child_fds(vars, vars->cmd.pipes);//fonction qui close tous les fds ouverts du processus fils
				setup_signals_subshell();
				vars->sa_setup = &setup_signals_subshell;
				res = search_binary_tree(vars, cmds->left, 0, 0);
				status = wait_processes(vars->cmd.last_exit_status, vars->cmd.last_pid);
				close_pipe(vars->cmd.pipes, 3);
				free_all(vars, NULL, false);
				exit(status);
			}
			else if (pid > 0)
			{
				vars->cmd.last_pid = pid;
				return (0);
			}
		}
		res = recursive_call(vars, cmds, (int[2]){pipe_in, pipe_out}, true);
		if (res == -1 || res == 130 || res == 131)
			return (res);
		if (cmds->operator_type != TOKEN_PIPE)
		{
			vars->cmd.last_exit_status = wait_processes
				(vars->cmd.last_exit_status, vars->cmd.last_pid);
			if (cmds->operator_type == TOKEN_OR)
			{
				if (vars->cmd.last_exit_status == 130
					|| vars->cmd.last_exit_status == 0
					|| vars->cmd.last_exit_status == 131)
					return (vars->cmd.last_exit_status);
			}
			else
			{
				if (vars->cmd.last_exit_status != 0)
					return (vars->cmd.last_exit_status);
			}
		}
		res = recursive_call(vars, cmds, (int[2]){pipe_in, pipe_out}, false);
		if (res == -1 || res == 130 || res == 131)
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
		return (130);
	else if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		return (131);
	}
	if (last_status == -1 || !last_known_pid)
		return (last_known_exit_status);
	return (WEXITSTATUS(last_status));
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
