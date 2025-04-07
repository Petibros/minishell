/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:36:35 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/07 02:28:04 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*get_path(char *cmd, char **envp)
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
		if (!path || access(path, F_OK) == 0)
			break ;
		free(path);
		++i;
	}
	if (to_join)
		free(to_join);
	if (!paths || !paths[i])
		path = NULL;
	free_string_array(paths);
	return (path);
}

static void	is_built_in(char **argv, char **envp)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		echo(argv, envp);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		env(argv, envp);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		pwd(argv, envp);
}

static int	is_exec(char *path)
{
	DIR	*dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		write(2, path, ft_strlen(path));
		write(2, ": is a directory\n", 17);
		return (1);
	}
	else if (access(path, X_OK) != 0)
	{
		perror(path);
		return (1);
	}
	return (0);
}

void	exec_cmd(t_vars *vars, t_nodes *cmds, int pipes[2][2])
{
	char	*path;
	char	**argv;
	char	**envp;

	argv = cmds->argv;
	envp = vars->env.envp;
	if (!argv || vars->cmd.fd_in == -1 || vars->cmd.fd_out == -1)
		exit_fd_error(vars, pipes);//fonction qui free tout les pointeurs du processus fils
	dup2(vars->cmd.fd_in, 0);
	dup2(vars->cmd.fd_out, 1);
	close_child_fds(vars, pipes);//fonction qui close tous les fds ouverts du processus fils
	free_all(vars, argv, true);
	is_built_in(argv, envp);
	if (ft_strchr(argv[0], '/'))//check si la commande est un chemin pre-etabli
		path = ft_strdup(argv[0]);
	else
		path = get_path(argv[0], envp);//cherche le path dans l'environnement
	execve(path, argv, envp);
	if (!path || access(path, F_OK) != 0)
		exit_error(path, envp, argv, 127);//command not found ou path pas trouve dans l'environnement
	else if (is_exec(path) != 0)
		exit_error(path, envp, argv, 126);//not enough privileges
	else
		exit_error(path, envp, argv, 2);//invalid option(2)
}
