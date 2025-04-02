/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "wildcard_expander.h"

static int	count_args_without_wildcard(char **argv)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (argv[i])
	{
		if (!has_unquoted_wildcard(argv[i]))
			count++;
		i++;
	}
	return (count);
}

static int	count_wildcard_matches(char **argv)
{
	int	i;
	int	count;
	int	tmp;

	count = 0;
	i = 0;
	while (argv[i])
	{
		if (has_unquoted_wildcard(argv[i]))
		{
			tmp = count_matching_entries(argv[i]);
			if (tmp > 0)
				count += tmp;
			else
				count++;
		}
		i++;
	}
	return (count);
}

static int	count_and_create_argv(char **argv, char ***new_argv)
{
	int	count;

	count = count_args_without_wildcard(argv) + count_wildcard_matches(argv);
	*new_argv = malloc(sizeof(char *) * (count + 1));
	if (!*new_argv)
		return (0);
	return (count);
}

void	expand_node_wildcards(t_nodes *node)
{
	char	**new_argv;
	int		count;
	int		i;
	int		j;

	if (!node || !node->argv)
		return ;
	count = count_and_create_argv(node->argv, &new_argv);
	if (count == 0 || !new_argv)
		return ;
	i = 0;
	j = 0;
	while (node->argv[i])
	{
		expand_wildcard_entry(node->argv[i], new_argv, &j);
		i++;
	}
	new_argv[j] = NULL;
	free_array(node->argv);
	node->argv = new_argv;
}

void	expand_wildcards(t_nodes *node)
{
	if (!node)
		return ;
	expand_node_wildcards(node);
	if (node->left)
		expand_wildcards(node->left);
	if (node->right)
		expand_wildcards(node->right);
}
