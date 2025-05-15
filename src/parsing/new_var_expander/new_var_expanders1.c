/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var_expanders1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:38:38 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*new_add_pre_str(char *current, char *pre_str)
{
	char	*result;

	result = ft_strjoin(current, pre_str);
	free(current);
	free(pre_str);
	return (result);
}

char	*new_get_var_name(char *dollar)
{
	char	*name;
	int		i;

	i = 1;
	while (dollar[i] && (ft_isalpha(dollar[i]) || dollar[i] == '_'))
		++i;
	while (dollar[i] && (ft_isalnum(dollar[i]) || dollar[i] == '_'))
		++i;
	name = ft_substr(dollar, 1, i - 1);
	return (name);
}

char	*new_get_var_value(char *dollar, char **envp, t_vars *vars)
{
	char	*name;
	char	*value;

	if (dollar[1] == '\0')
	{
		return (ft_strdup("$"));
	}
	if (dollar[1] == '?')
		return (ft_itoa(vars->cmd.last_exit_status));
	name = new_get_var_name(dollar);
	value = ft_getenv(envp, name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*new_skip_var(char *dollar)
{
	int	i;

	i = 1;
	if (dollar[1] == '?')
	{
		return (dollar + 2);
	}
	while (dollar[i] && (ft_isalpha(dollar[i]) || dollar[i] == '_'))
		++i;
	while (dollar[i] && (ft_isalnum(dollar[i]) || dollar[i] == '_'))
		++i;
	return (dollar + i);
}

char	*new_find_next_expansion(char *str, int *is_in_double_quote)
{
	while (str && *str)
	{
		if (*str == '"')
			*is_in_double_quote = !*is_in_double_quote;
		if (*str == '$'
			&& (ft_isalpha(str[1]) || str[1] == '_' || str[1] == '?'))
			return (str);
		if (*str == '\'' && !*is_in_double_quote)
			str = ft_strchr(str + 1, '\'');
		if (str)
			++str;
	}
	return (NULL);
}
