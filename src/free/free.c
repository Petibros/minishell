/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:30:04 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/28 00:46:47 by sacha            ###   ########.fr       */
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
		++i;
	}
	free(ptr);
}

void	free_redir(t_redir *node)
{
	t_redir	*tmp_node;

	while (node)
	{
		free(node->filename);
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
		free_redir(tree->file_in);
		free_redir(tree->heredoc);
		free_redir(tree->file_out);
		if (tree->argv != to_not_free)
			free_string_array(tree->argv);
		node = tree->left;
		free(tree);
		tree = node;
	}
}

void	free_all(t_vars *vars, char **to_not_free, bool in_child)
{
	if (vars->line)
		free(vars->line);
	if (vars->prompt)
		free(vars->prompt);
	if (in_child == false)
		free_string_array(vars->env.envp);
	free_branch(vars->cmd.cmds, to_not_free);
	if (vars)
		free(vars);
}

void	close_fds(int pipes[2][2], t_vars *vars)
{
	if (pipes[vars->cmd.pipes_count % 2][1] == vars->cmd.fd_out)
	{
		pipes[vars->cmd.pipes_count % 2][1] = 0;
		if (pipes[(vars->cmd.pipes_count + 1) % 2][0] == vars->cmd.fd_in)
			vars->cmd.fd_in = 0;
		close_pipe(pipes, (vars->cmd.pipes_count + 1) % 2 + 1);
	}
	else
	{
		if (pipes[vars->cmd.pipes_count % 2][0] == vars->cmd.fd_in)
			vars->cmd.fd_in = 0;
		close_pipe(pipes, vars->cmd.pipes_count % 2 + 1);
	}
	if (vars->cmd.fd_in > 2)
		close(vars->cmd.fd_in);
	if (vars->cmd.fd_out > 2)
		close(vars->cmd.fd_out);
}
