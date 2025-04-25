/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:35:08 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:35:09 by sacgarci         ###   ########.fr       */
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

static char	*search_correct_path(char *to_join, char **paths, int *status)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
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
	return (path);
}

char	*get_path(char *cmd, char **envp, int *status)
{
	int		i;
	char	**paths;
	char	*path;
	char	*to_join;

	i = path_index(envp);
	if (i == -1)
		return (ft_strjoin("./", cmd));
	paths = ft_split(&envp[i][5], ':');
	to_join = ft_strjoin("/", cmd);
	path = search_correct_path(to_join, paths, status);
	free_string_array(paths);
	if (path)
		*status = 0;
	return (path);
}
