/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:09:00 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 17:09:00 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

static void	expand_single_arg(char **arg, int exit_status, char **envp)
{
	char	*expanded;

	if (ft_strchr(*arg, '$'))
	{
		expanded = expand_variables(*arg, exit_status, envp);
		if (expanded)
		{
			free(*arg);
			*arg = expanded;
		}
	}
}

static void	expand_argv(char **argv, int exit_status, char **envp)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		expand_single_arg(&argv[i], exit_status, envp);
		i++;
	}
}

void	expand_variables_in_node(t_nodes *node, int exit_status, char **envp)
{
	if (!node)
		return ;
	if (node->argv)
	{
		expand_argv(node->argv, exit_status, envp);
	}
	if (node->left)
		expand_variables_in_node(node->left, exit_status, envp);
	if (node->right)
		expand_variables_in_node(node->right, exit_status, envp);
}
