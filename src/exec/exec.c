/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/16 20:28:39 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	first_fd_in(t_vars *vars, t_nodes **cmds)
{
	int		fd_in;
	t_redir	*redir;
	char	*heredoc_path;

	fd_in = 0;
	if ((*cmds)->file_in)
	{
		redir = (*cmds)->file_in;
		while (redir->next)
			redir = redir->next;
		fd_in = open(redir->filename, O_RDONLY);
		if (fd_in == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(redir->filename, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-2);
		}
	}
	else if ((*cmds)->heredoc)
	{
		redir = (*cmds)->heredoc;
		while (redir->next)
			redir = redir->next;
		heredoc_path = get_tmp();
		if (!heredoc_path)
			return (-2);
		fd_in = open(heredoc_path, O_CREAT | O_WRONLY | O_TRUNC, 0700);
		if (fd_in == -1)
		{
			free(heredoc_path);
			return (-2);
		}
		here_doc(fd_in, redir->filename);
		close(fd_in);
		fd_in = open(heredoc_path, O_RDONLY);
		unlink(heredoc_path);
		free(heredoc_path);
	}
	return (fd_in);
}

static void	init_pipes(int pipes[2][2])
{
	pipes[0][0] = 0;
	pipes[0][1] = 0;
	pipes[1][0] = 0;
	pipes[1][1] = 0;
}

int	execute(t_vars *vars, t_nodes **cmds)
{
	int	pipes[2][2];

	init_pipes(pipes);
	vars->cmd.pipes_count = 0;
	vars->cmd.fd_in = first_fd_in(vars, cmds);
	if (vars->cmd.fd_in == -2)
		return (-1);
	while (*cmds)
	{
		if (exec_routine(vars, cmds, pipes) == -1)
			return (-1);
		if (*cmds->last_exit_status != 0 && *cmds->right)//si la commande est la pre;iere d'un || et fail
			*cmds = *cmds->right;
		else
			*cmds = *cmds->left;
	}
	close_pipes(pipes, 3);
	return (0);
}
