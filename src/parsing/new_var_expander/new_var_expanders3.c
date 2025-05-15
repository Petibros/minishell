/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var_expanders3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:38:38 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_expand_argv(char **argv, char **envp, t_vars *vars)
{
	int		i;
	char	*tmp;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		tmp = argv[i];
		argv[i] = new_get_expanded_str(argv[i], envp, vars);
		free(tmp);
		if (!argv[i] || argv[i][0] == '\0')
		{
			join_hole(argv + i);
			continue ;
		}
		++i;
	}
}

void	new_expand_redirs(t_redir *redirs, t_vars *vars)
{
	char	*tmp;

	if (!redirs)
		return ;
	while (redirs)
	{
		redirs->old_filename = ft_strdup(redirs->filename);
		tmp = redirs->filename;
		redirs->filename
			= new_get_expanded_str(redirs->filename, vars->env.envp, vars);
		free(tmp);
		redirs = redirs->next;
	}
}

void	new_expand_variables_in_node(t_nodes *node, t_vars *vars)
{
	char	**argv;
	char	**new_argv;

	if (!node)
		return ;
	argv = node->argv;
	if (argv)
	{
		new_argv = new_expand_argv_with_split(argv, vars->env.envp, vars);
		new_free_arr(argv);
		node->argv = new_argv;
	}
	if (node->file_in)
		new_expand_redirs(node->file_in, vars);
	if (node->file_out)
		new_expand_redirs(node->file_out, vars);
}
