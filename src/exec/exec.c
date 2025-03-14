/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/14 16:12:48 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute(t_vars *vars)
{
	int	n;
	int	pipes[2][2];

	vars->cmd.pipes_count = 0;
	n = 1;
	while (n <= vars->cmd.cmd_count)
	{
		if (vars->cmd.next_operator[n - 1] == PIPE || n == 1)
		{
			pipe_routine(vars, pipes, n);
		}
		++n;
	}
}
