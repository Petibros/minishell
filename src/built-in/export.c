/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:48:28 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/29 05:04:35 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	already_exists(char **argv, char **envp, int var_len)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(argv[1], envp[i], var_len) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strdup(argv[1]);
			if (!envp[i])
			{
				perror("export.c, already_exists(), l.25");
				++i;
				while (envp[i])
				{
					free(envp[i]);
					++i;
				}
				return (-1);
			}
			return (1);
		}
		++i;
	}
	return (0);
}

static char	**alloc_new_array(char **envp, int size)
{
	int		i;
	char	**new_array;

	i = 0;
	new_array = malloc((size + 1) * sizeof(char *));
	while (new_array && envp[i])
	{
		new_array[i] = ft_strdup(envp[i]);
		if (!new_array[i])
		{
			--i;
			while (i > 0)
			{
				free(new_array[i]);
				--i;
			}
			free(new_array);
			new_array = NULL;
		}
		++i;
	}
	free_string_array(envp);
	return (new_array);
}

static int	is_valid(char **argv)
{
	int	i;

	i = 0;
	while (argv[1][i] && (ft_isalpha(argv[1][i]) || argv[1][i] == '_'))
		++i;
	if (argv[1][i] != '=' || i == 0)
	{
		write(2, "minishell: export: ", 19);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": not a valid identifier\n", 25);
		return (-1);
	}
	return (i + 1);
}

static int	solely_export(char **envp)
{
	int	i;
	int	var_len;

	i = 0;
	while (envp[i])
	{
		var_len = ft_strnlen(envp[i], '=') + 1;
		printf("declare -x %.*s\"%s\"\n", var_len, envp[i], &envp[i][var_len]);
		++i;
	}
	return (1);
}

int	export_var(char **argv, char ***envp, t_vars *vars)
{
	int	status;
	int	i;

	if (!argv[1])
		return (solely_export(*envp));
	vars->cmd.last_exit_status = 0;
	i = -1;
	while (argv[i + 2])
	{
		++i;
		status = is_valid(&argv[i]);
		if (status == -1)
		{
			vars->cmd.last_exit_status = 1;
			continue ;
		}
		status = already_exists(&argv[i], *envp, ft_strnlen(argv[i + 1], '=') + 1);
		if (status == 1)
			continue ;
		else if (status == -1)
			return (-1);
		if (vars->env.current_size >= vars->env.alloc_size)
		{
			++vars->env.alloc_size;
			*envp = alloc_new_array(*envp, vars->env.alloc_size);
			if (!*envp)
			{
				perror("export.c, alloc_new_array");
				return (-1);
			}
		}
		(*envp)[vars->env.current_size + 1] = NULL;
		(*envp)[vars->env.current_size] = ft_strdup(argv[i + 1]);
		if (!(*envp)[vars->env.current_size])
		{
			perror("export.c, l.119");
			return (-1);
		}
		++vars->env.current_size;
	}
	return (1);
}
