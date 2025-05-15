/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var_expanders2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:38:38 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_handle_expansions(char **str, char **envp, t_vars *vars,
			int *is_in_double_quote)
{
	char	*current;
	char	*dollar;
	char	*pre_str;
	char	*var_value;
	char	*tmp;

	current = NULL;
	dollar = new_find_next_expansion(*str, is_in_double_quote);
	while (**str && dollar)
	{
		pre_str = ft_substr(*str, 0, dollar - *str);
		current = new_add_pre_str(current, pre_str);
		var_value = new_get_var_value(dollar, envp, vars);
		tmp = current;
		current = ft_strjoin(current, var_value);
		free(tmp);
		free(var_value);
		*str = new_skip_var(dollar);
		dollar = new_find_next_expansion(*str, is_in_double_quote);
	}
	return (current);
}

char	*new_get_expanded_str(char *str, char **envp, t_vars *vars)
{
	char	*current;
	char	*pre_str;
	int		is_in_double_quote;

	if (!str)
		return (NULL);
	is_in_double_quote = 0;
	current = new_handle_expansions(&str, envp, vars, &is_in_double_quote);
	if (!(*str))
		return (current);
	pre_str = ft_strdup(str);
	current = new_add_pre_str(current, pre_str);
	return (current);
}

void	join_hole(char **argv)
{
	int	i;

	free(argv[0]);
	argv[0] = NULL;
	i = 0;
	while (argv[i + 1])
	{
		argv[i] = argv[i + 1];
		++i;
	}
	argv[i] = NULL;
}

char	**new_expand_single_argv(char *arg, char **envp, t_vars *vars,
		char **result)
{
	char	*expanded;
	char	**split_result;
	char	**new_result;

	expanded = new_get_expanded_str(arg, envp, vars);
	if (!expanded || expanded[0] == '\0')
	{
		free(expanded);
		return (result);
	}
	split_result = new_split_expanded_string(expanded);
	if (split_result)
	{
		new_result = new_join_string_arrays(result, split_result);
		new_free_arr(result);
		new_free_arr(split_result);
		result = new_result;
		if (!result)
		{
			free(expanded);
			return (NULL);
		}
	}
	free(expanded);
	return (result);
}

char	**new_expand_argv_with_split(char **argv, char **envp, t_vars *vars)
{
	int		i;
	char	**result;

	if (!argv)
		return (NULL);
	result = malloc(sizeof(char *) * 1);
	if (!result)
		return (NULL);
	result[0] = NULL;
	i = 0;
	while (argv[i])
	{
		result = new_expand_single_argv(argv[i], envp, vars, result);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
