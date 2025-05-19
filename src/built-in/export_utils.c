/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:33:52 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/18 14:54:49 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_existing(char **envp, char **argv, int i, int var_len)
{
	if (ft_strncmp(argv[1], envp[i], var_len) == 0
		&& (envp[i][var_len] == '=' || !envp[i][var_len]))
		return (true);
	return (false);
}

int	unset_null(char **envp, int i, int *current_size)
{
	perror("export.c, already_exists()");
	--(*current_size);
	while (envp[i + 1])
	{
		envp[i] = envp[i + 1];
		envp[i + 1] = NULL;
		++i;
	}
	return (-1);
}

int	solely_export(char **envp)
{
	int	i;
	int	var_len;

	i = 0;
	while (envp[i])
	{
		var_len = ft_strnlen(envp[i], '=') + 1;
		if (!envp[i][var_len - 1])
			printf("declare -x %s\n", envp[i]);
		else if (var_len != 2 || envp[i][0] != '_')
			printf("declare -x %.*s\"%s\"\n",
				var_len, envp[i], &envp[i][var_len]);
		++i;
	}
	return (0);
}

char	*get_var(char *argv)
{
	int		var_len;
	char	*var;
	char	*value;
	char	*env;

	var_len = ft_strnlen(argv, '=') - 1;
	if (argv[var_len] == '+')
	{
		var = ft_substr(argv, 0, var_len);
		value = &argv[var_len + 1];
		env = ft_strjoin(var, value);
		if (var)
			free(var);
		return (env);
	}
	else
		return (ft_strdup(argv));
}
