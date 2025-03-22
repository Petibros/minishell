/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include "expander.h"

char	*expand_env_var(char *str, int *i, int exit_status)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		len;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	len = 0;
	while (ft_isalnum(str[*i + len]) || str[*i + len] == '_')
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(str, *i, len);
	*i += len;
	var_value = getenv(var_name);
	if (!var_value)
		result = ft_strdup("");
	else
		result = ft_strdup(var_value);
	free(var_name);
	return (result);
}

char	*expand_variables(char *str, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			result = handle_single_quote(str, &i, result);
		else if (str[i] == '"')
			result = handle_double_quote(str, &i, result, exit_status);
		else if (str[i] == '$')
		{
			tmp = expand_env_var(str, &i, exit_status);
			if (tmp)
			{
				result = ft_strjoin_free(result, tmp);
				free(tmp);
			}
		}
		else
			result = handle_regular_char(str, &i, result);
	}
	return (result);
}

static void	expand_single_arg(char **arg, int exit_status)
{
	char	*expanded;

	if (ft_strchr(*arg, '$'))
	{
		expanded = expand_variables(*arg, exit_status);
		if (expanded)
		{
			free(*arg);
			*arg = expanded;
		}
	}
}

static void	expand_argv(char **argv, int exit_status)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		expand_single_arg(&argv[i], exit_status);
		i++;
	}
}

void	expand_variables_in_node(t_nodes *node, int exit_status)
{
	if (!node)
		return ;
	if (node->argv)
	{
		expand_argv(node->argv, exit_status);
		if (node->cmd && node->argv[0])
		{
			free(node->cmd);
			node->cmd = ft_strdup(node->argv[0]);
		}
	}
	if (node->left)
		expand_variables_in_node(node->left, exit_status);
	if (node->right)
		expand_variables_in_node(node->right, exit_status);
}
