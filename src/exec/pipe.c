/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:44 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/14 17:07:40 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(int pipes[2][2], int	to_close)
{
	if (to_close == 1)
	{
		close(pipes[0][0]);
		close(pipes[0][1]);
	}
	else if (to_close == 2)
	{
		close(pipes[1][0]);
		close(pipes[1][1]);
	}
	else if (to_close == 3)
	{
		close_pipe(pipes, 1);
		close_pipe(pipes, 2);
	}
}

int	pipe_routine(t_vars *vars, int pipes[2][2], int n, int pipes_count)
{
	if (pipes_count == 0)
	{
		pipe(pipes[0]);
		pipe(pipes[1]);
	}
	else if ( % 2 == 1)
	else if (n % 2 == 0)
}
