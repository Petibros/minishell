/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:36:35 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/18 16:13:52 by sacha            ###   ########.fr       */
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
		status = export_var(argv, &envp, vars);
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

static int	is_exec(char *path, int *status)
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

static int	path_index(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		++i;
	if (!envp[i])
		return (-1);
	return (i);
}

static char	*get_path(char *cmd, char **envp, int *status)
{
	int		i;
	char	**paths;
	char	*path;
	char	*to_join;

	i = path_index(envp);
	if (i == -1)
		return (ft_strjoin("./", cmd));
	paths = ft_split(&envp[i][5], ':');
	i = 0;
	to_join = ft_strjoin("/", cmd);
	while (paths && paths[i])
	{
		path = ft_strjoin(paths[i], to_join);
		if (!path || is_exec(path, status) == 0)
			break ;
		free(path);
		++i;
	}
	if (to_join)
		free(to_join);
	if (!paths || !paths[i])
		path = NULL;
	free_string_array(paths);
	if (path)
		*status = 0;
	return (path);
}

void	exec_cmd(t_vars *vars, t_nodes *cmds)
{
	char	*path;
	char	**argv;
	char	**envp;
	int		status;

	path = NULL;
	status = 1;
	argv = cmds->argv;
	envp = vars->env.envp;
	dup2(vars->cmd.fd_in, 0);
	dup2(vars->cmd.fd_out, 1);
	close_child_fds(vars, vars->cmd.pipes);//fonction qui close tous les fds ouverts du processus fils
	is_built_in(argv, envp, vars);
	free_all(vars, argv, true);
	if (ft_strchr(argv[0], '/'))
		path = ft_strdup(argv[0]);
	else if (argv[0][0])
		path = get_path(argv[0], envp, &status);//cherche le path dans l'environnement
	if (status == 0 || is_exec(path, &status) == 0)
	{
		execve(path, argv, envp);
		exit_error(path, envp, argv, 2);//invalid option(2)
	}
	exit_error(path, envp, argv, status);//command not found ou path pas trouve dans l'environnement
}
