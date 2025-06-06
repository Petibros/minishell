/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:34:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/06/02 16:02:39 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_digits(int nbr)
{
	int	n;

	if (!nbr)
		return (1);
	n = 0;
	while (nbr)
	{
		nbr /= 10;
		++n;
	}
	return (n);
}

void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

int	subshell_and_clean(t_vars *vars, t_nodes *cmds, int pipe_in, int pipe_out)
{
	int	status;

	status = create_subshell(vars, cmds, pipe_in, pipe_out);
	close_fds(vars);
	return (status);
}

int	print_quit(void)
{
	printf("Quit (core dumped)\n");
	return (131);
}
