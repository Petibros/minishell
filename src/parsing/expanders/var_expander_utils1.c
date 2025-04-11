/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:09:00 by npapash           #+#    #+#             */
/*   Updated: 2025/04/12 01:28:51 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

static char	*append_expanded_segment(char *result, char *segment)
{
	char	*expanded;

	expanded = ft_strjoin(result, segment);
	free(segment);
	free(result);
	return (expanded);
}

static char	*handle_dollar_expansion(char *arg, int exit_status, char **envp)
{
	char	*result;
	char	*tmp;
	int		i;
	int		j;

	result = ft_strdup("");
	i = 0;
	while (arg[i])
	{
		j = i;
		if (arg[i] == '$')
		{
			tmp = expand_env_var(arg, &i, exit_status, envp);
			result = append_expanded_segment(result, tmp);
			continue ;
		}
		while (arg[i] && arg[i] != '$')
			i++;
		tmp = ft_substr(arg, j, i - j);
		result = append_expanded_segment(result, tmp);
	}
	free(arg);
	return (result);
}

static void	shift_argv(char **argv, int i, int *j)
{
	if (i != *j)
		argv[*j] = argv[i];
	(*j)++;
}

static void	expand_argv(char **argv, int exit_status, char **envp)
{
	int		i;
	int		j;
	char	*expanded;

	i = 0;
	j = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '$'))
		{
			expanded = handle_dollar_expansion(argv[i], exit_status, envp);
			if (expanded)
			{
				argv[j] = expanded;
				j++;
			}
		}
		else
			shift_argv(argv, i, &j);
		i++;
	}
	argv[j] = NULL;
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
