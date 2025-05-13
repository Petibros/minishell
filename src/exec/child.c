/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:36:35 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 18:05:49 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_built_in(char **argv, char **envp, t_vars *vars)
{
	int	status;

	status = -10;
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		status = echo(argv, envp);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		status = env(argv, envp);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		status = pwd(argv, envp);
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		status = export_var(argv, &vars->env.envp, vars);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		status = unset(argv, vars);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		status = cd(argv, vars);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		status = exit_built_in(argv, vars, false);
	if (status != -10)
	{
		free_all(vars, NULL, false);
		exit(status);
	}
}

int	is_exec(char *path, int *status)
{
	DIR	*dir;

	if (!path || access(path, F_OK) != 0)
	{
		if (*status != 126 && *status != 128)
			*status = 127;
		return (1);
	}
	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		*status = 128;
		return (1);
	}
	else if (access(path, X_OK) != 0)
	{
		*status = 126;
		return (1);
	}
	return (0);
}

void	exec_cmd(t_vars *vars, t_nodes *cmds)
{
	char	*path;
	char	**argv;
	char	**envp;
	int		status;

	path = NULL;
	status = 1;
	envp = vars->env.envp;
	dup2(vars->cmd.fd_in, 0);
	dup2(vars->cmd.fd_out, 1);
	
	close_child_fds(vars, vars->cmd.pipes);

	argv = cmds->argv;
	is_built_in(argv, envp, vars);
	free_all(vars, argv, true);
	if (ft_strchr(argv[0], '/'))
		path = ft_strdup(argv[0]);
	else if (argv[0][0])
		path = get_path(argv[0], envp, &status);
	if (status == 0 || is_exec(path, &status) == 0)
	{
		execve(path, argv, envp);
		exit_error(path, envp, argv, 2);
	}
	exit_error(path, envp, argv, status);
}
