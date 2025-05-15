/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_wildcard_expanders3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:38:38 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**handle_expansion(char **result, char *str)
{
	char	*expanded;
	char	**temp;

	expanded = new_expand_wildcard(str);
	if (!expanded)
		return (result);
	temp = new_split_expanded_string(expanded);
	free(expanded);
	if (!temp)
		return (result);
	return (merge_and_free(result, temp));
}

void	new_expand_redirs_wildcards(t_redir *redirs, t_vars *vars, int *status)
{
	char	*tmp;
	char	**check_expand;

	if (!redirs)
		return ;
	while (redirs && *status != 2)
	{
		tmp = redirs->filename;
		redirs->filename = new_expand_wildcard(redirs->filename);
		check_expand = supra_split(redirs->filename);
		if (!check_expand || (check_expand[0] && check_expand[1]))
		{
			*status = 2;
			vars->cmd.last_exit_status = 1;
			if (redirs->old_filename)
				write(2, redirs->old_filename, ft_strlen(redirs->old_filename));
			write(2, ": ambiguous redirect\n", 21);
		}
		free_string_array(check_expand);
		free(tmp);
		if (redirs->old_filename)
			free(redirs->old_filename);
		redirs = redirs->next;
	}
}

void	new_expand_wildcards_in_node(t_nodes *node, t_vars *vars, int *status)
{
	char	**argv;

	if (!node)
		return ;
	argv = node->argv;
	if (argv)
	{
		node->argv = new_expand_wildcards_array(argv);
		new_free_arr(argv);
	}
	if (node->file_in)
		new_expand_redirs_wildcards(node->file_in, vars, status);
	if (node->file_out)
		new_expand_redirs_wildcards(node->file_out, vars, status);
}
