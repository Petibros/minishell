/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 22:39:43 by npapash           #+#    #+#             */
/*   Updated: 2025/04/11 22:39:43 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"
#include "libft.h"

char	*get_var_value(char *var_name, int exit_status, char **envp)
{
	char	*var_value;

	if (!ft_strncmp(var_name, "?", 2))
		return (ft_itoa(exit_status));
	if (!ft_strncmp(var_name, "_", 2))
	{
		var_value = ft_getenv(envp, "_");
		if (!var_value)
			return (NULL);
		return (ft_strdup(var_value));
	}
	var_value = ft_getenv(envp, var_name);
	if (!var_value)
		return (NULL);
	return (ft_strdup(var_value));
}

char	*get_var_name(char *str, int *i)
{
	int		len;
	char	*var_name;

	len = 0;
	if (str[*i] == '_' || ft_isalpha(str[*i]))
	{
		len++;
		while (str[*i + len])
		{
			if (!ft_isalnum(str[*i + len]) && str[*i + len] != '_')
				break ;
			len++;
		}
	}
	if (len == 0)
		return (ft_strdup(""));
	var_name = ft_substr(str, *i, len);
	*i += len;
	return (var_name);
}

char	*handle_special_var_cases(char *str, int *i, int exit_status)
{
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	if (str[*i] == '\'')
		return (ft_strdup(""));
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
	{
		if (str[*i] == '"' && *(i) == 1)
			return (ft_strdup(""));
		return (ft_strdup("$"));
	}
	return (NULL);
}
