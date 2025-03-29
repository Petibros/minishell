/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:08:45 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/29 05:19:32 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_child_fds(t_vars *vars, int pipes[2][2])
{
	if (vars->cmd.fd_in == pipes[0][0] || vars->cmd.fd_in == pipes[1][0])
		vars->cmd.fd_in = 0;
	if (vars->cmd.fd_out == pipes[0][1] || vars->cmd.fd_out == pipes[1][1])
		vars->cmd.fd_out = 0;
	close_pipe(pipes, 3);
	if (vars->cmd.fd_in > 2)
		close(vars->cmd.fd_in);
	if (vars->cmd.fd_out > 2)
		close(vars->cmd.fd_out);
}

void	exit_fd_error(t_vars *vars, int pipes[2][2])
{
	close_child_fds(vars, pipes);
	free_all(vars, NULL, false);
	exit(1);
}

void	exit_error(char *path, char **envp, char **argv, int status)
{
	if (status == 127)
	{
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": command not found\n", 20);
	}
	else
		perror(argv[0]);
	free_string_array(envp);
	free_string_array(argv);
	if (path)
		free(path);
	exit(status);
}
