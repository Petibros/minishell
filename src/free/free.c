/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:30:04 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/15 14:23:49 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_string_array(char **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		++i;
	}
	free(ptr);
	ptr = NULL;
}

void	free_redir(t_redir *node)
{
	t_redir	*tmp_node;

	while (node)
	{
		if (node->filename)
			free(node->filename);
		node->filename = NULL;
		tmp_node = node->next;
		free(node);
		node = tmp_node;
	}
}

void	free_branch(t_nodes *tree, char **to_not_free)
{
	t_nodes	*node;

	while (tree)
	{
		if (tree->right)
			free_branch(tree->right, to_not_free);
		tree->right = NULL;
		free_redir(tree->file_in);
		free_redir(tree->heredoc);
		free_redir(tree->file_out);
		tree->file_in = NULL;
		tree->file_out = NULL;
		tree->heredoc = NULL;
		if (tree->argv != to_not_free)
		{
			free_string_array(tree->argv);
			tree->argv = NULL;
		}
		node = tree->left;
		free(tree);
		tree = node;
	}
}

void	free_all(t_vars *vars, char **to_not_free, bool in_child)
{
	if (!vars)
		return ;
	if (vars->line)
		free(vars->line);
	vars->line = NULL;
	if (vars->prompt)
		free(vars->prompt);
	vars->prompt = NULL;
	if (in_child == false)
	{
		free_string_array(vars->env.envp);
		vars->env.envp = NULL;
	}
	free_branch(vars->cmd.cmds, to_not_free);
	vars->cmd.cmds = NULL;
	if (vars)
		free(vars);
	vars = NULL;
}
