/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:08:45 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/20 00:07:33 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_child_fds(t_vars *vars, int pipes[2][2])
{
	close_pipe(pipes, 3);
	if (*vars->cmd.fd_in > 0)
		close(*vars->cmd.fd_in);
	if (*vars->cmd.fd_out > 0)
		close(*vars->cmd.fd_out);
}

void	exit_fd_error(t_vars *vars, int pipes[2][2])
{
	close_child_fds(vars, pipes);
	free_all(vars, NULL, false);
	exit(1);
}

void	exit_error(char *path, char **envp, char **argv, int status)
{
	perror(path);
	free_string_array(envp);
	free_string_array(argv);
	if (path)
		free(path);
	exit(status);
}
