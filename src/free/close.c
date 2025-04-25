/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:34:48 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:34:48 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(int pipes[2][2], int to_close)
{
	if (to_close == 1)
	{
		if (pipes[0][0] > 0)
			close(pipes[0][0]);
		pipes[0][0] = 0;
		if (pipes[0][1] > 0)
			close(pipes[0][1]);
		pipes[0][1] = 0;
	}
	else if (to_close == 2)
	{
		if (pipes[1][0] > 0)
			close(pipes[1][0]);
		pipes[1][0] = 0;
		if (pipes[1][1] > 0)
			close(pipes[1][1]);
		pipes[1][1] = 0;
	}
	else if (to_close == 3)
	{
		close_pipe(pipes, 1);
		close_pipe(pipes, 2);
	}
}

void	close_fds(t_vars *vars)
{
	if (vars->cmd.pipes[vars->cmd.pipes_count % 2][1] == vars->cmd.fd_out)
	{
		vars->cmd.pipes[vars->cmd.pipes_count % 2][1] = 0;
		if (vars->cmd.pipes[(vars->cmd.pipes_count + 1) % 2][0] ==
			vars->cmd.fd_in)
			vars->cmd.fd_in = 0;
		close_pipe(vars->cmd.pipes, (vars->cmd.pipes_count + 1) % 2 + 1);
	}
	else
	{
		if (vars->cmd.pipes[vars->cmd.pipes_count % 2][0] == vars->cmd.fd_in)
			vars->cmd.fd_in = 0;
		close_pipe(vars->cmd.pipes, vars->cmd.pipes_count % 2 + 1);
	}
	if (vars->cmd.fd_in > 2)
		close(vars->cmd.fd_in);
	if (vars->cmd.fd_out > 2)
		close(vars->cmd.fd_out);
}
