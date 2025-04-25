/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_descriptors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:35:02 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:35:03 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_fd(t_redir **redirs, int *fd, int fd_type, t_vars *vars)
{
	t_redir	*files;

	files = *redirs;
	while (files && *fd != -1)
	{
		if (fd_type == 2)
			heredoc_gestion(vars, files, fd);
		else if (fd_type == 3 && files->append)
			*fd = open(files->filename, O_WRONLY | O_APPEND | O_CREAT, 0777);
		else if (fd_type == 3)
			*fd = open(files->filename, O_WRONLY | O_TRUNC | O_CREAT, 0777);
		else if (fd_type == 1)
			*fd = open(files->filename, O_RDONLY);
		if (*fd == -1)
			perror(files->filename);
		files = files->next;
		if (files && *fd != -1)
			close(*fd);
	}
}

static void	get_fd_in(t_vars *vars, t_nodes *cmds, int is_pipe[2], int *fd_in)
{
	*fd_in = 0;
	if (cmds->heredoc)
		open_fd(&cmds->heredoc, fd_in, 2, vars);
	if (*fd_in <= -2)
		return ;
	if (cmds->file_in)
		open_fd(&cmds->file_in, fd_in, 1, vars);
	else if (is_pipe[0] == 1)
		*fd_in = vars->cmd.pipes[vars->cmd.pipes_count % 2][0];
}

static void	get_fd_out(t_vars *vars, t_nodes *cmds, int is_pipe[2], int *fd_out)
{
	*fd_out = 1;
	if (cmds->file_out)
		open_fd(&cmds->file_out, fd_out, 3, vars);
	else if (is_pipe[1] == 1)
	{
		++vars->cmd.pipes_count;
		pipe(vars->cmd.pipes[vars->cmd.pipes_count % 2]);
		*fd_out = vars->cmd.pipes[vars->cmd.pipes_count % 2][1];
	}
}

int	get_fds(t_vars *vars, t_nodes *cmds, int is_pipe[2])
{
	get_fd_in(vars, cmds, is_pipe, &vars->cmd.fd_in);
	get_fd_out(vars, cmds, is_pipe, &vars->cmd.fd_out);
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
		vars->cmd.last_exit_status = 1;
		return (1);
	}
	return (0);
}
