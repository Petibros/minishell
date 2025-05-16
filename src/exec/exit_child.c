/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:08:45 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/16 14:01:08 by sacgarci         ###   ########.fr       */
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

void	exit_error(char *path, char **envp, char **argv, int status)
{
	if (status >= 126 && status <= 128)
		write(2, argv[0], ft_strlen(argv[0]));
	if (status == 127)
		write(2, ": command not found\n", 20);
	if (status == 126 || status == 128)
	{
		if (status == 128)
			write(2, ": is a directory\n", 17);
		else
			write(2, ": permission denied\n", 20);
		status = 126;
	}
	else if (status == 2)
		perror(argv[0]);
	free_string_array(envp);
	free_string_array(argv);
	if (path)
		free(path);
	exit(status);
}
