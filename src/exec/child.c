/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 21:36:35 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/20 00:07:51 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//BIEN PENSER A FAIRE LES FREE ET CLOSES QUAND TU AURAS FINI

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
		return (NULL);
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
	free_string_array(paths);
	if (to_join)
		free(to_join);
	if (!paths || !paths[i])
		return (NULL);
	return (path);
}

void	exec_cmd(t_vars *vars, t_nodes *cmds, int pipes[2][2])
{
	char	*path;
	char	**argv;
	char	**envp;

	argv = cmds->argv;
	envp = vars->env.envp;
	if (*vars->cmd.fd_in == -1 || *vars->cmd.fd_out == -1)
		exit_fd_error(vars, pipes);//fonction qui free tout les pointeurs du processus fils
	dup2(*vars->cmd.fd_in, 0);
	dup2(*vars->cmd.fd_out, 1);
	close_child_fds(vars, pipes);//fonction qui close tous les fds ouverts du processus fils
	if (ft_strchr(cmds->argv[0], '/'))//check si la commande est un chemin pre-etabli
		path = ft_strdup(path);
	else
		path = get_path(cmds->argv[0], vars->env.envp);//cherche le path dans l'environnement
	free_all(vars, argv, true);
	execve(path, argv, envp);
	if (path && path[0] == '.' && path[1] == '/')//si c'est un executable
		path = &path[2];
	if (!path || access(path, F_OK) != 0)
		exit_error(path, envp, argv, 127);//command not found ou path pas trouve dans l'environnement
	else if (access(path, X_OK) != 0)
		exit_error(path, envp, argv, 126);//not enough privileges
	else
		exit_error(path, envp, argv, 2);//invalid option(2)
}
