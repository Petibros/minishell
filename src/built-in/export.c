/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:48:28 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/28 19:33:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	already_exists(char **argv, char **envp,
		int var_len, int concatenate)
{
	int		i;
	char	*tmp;

	if (argv[1][var_len] == '+')
		concatenate = 1;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(argv[1], envp[i], var_len) == 0
			&& envp[i][var_len] == '=')
		{
			if (concatenate)
				tmp = ft_strjoin(envp[i], &argv[1][var_len + 2]);
			else
				tmp = ft_strdup(argv[1]);
			free(envp[i]);
			envp[i] = tmp;
			if (!envp[i])
				return (i);
			return (-1);
		}
		++i;
	}
	return (-2);
}

static char	**alloc_new_array(char **envp, int size)
{
	int		i;
	char	**new_array;

	i = 0;
	new_array = malloc((size + 1) * sizeof(char *));
	if (!new_array)
	{
		perror("export.c: alloc_new_array()");
		return (NULL);
	}
	while (envp[i])
	{
		new_array[i] = envp[i];
		++i;
	}
	free(envp);
	return (new_array);
}

static int	is_valid(char **argv, int *status)
{
	int	i;

	i = 0;
	while (argv[1][i] && (ft_isalnum(argv[1][i]) || argv[1][i] == '_')
		&& !ft_isdigit(argv[1][0]))
		++i;
	if ((argv[1][i] != '=' && !(argv[1][i] == '+'
		&& argv[1][i + 1] == '=')) || i == 0)
	{
		if (!argv[1][i] && i > 0)
			return (-1);
		write(2, "minishell: export: ", 19);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": not a valid identifier\n", 25);
		*status = 1;
		return (-1);
	}
	return (i);
}

static int	add_var(char **argv, char ***envp, t_vars *vars, int i)
{
	char	**tmp_array;

	if (vars->env.current_size >= vars->env.alloc_size)
	{
		tmp_array = alloc_new_array(*envp, vars->env.alloc_size);
		if (!tmp_array)
			return (-1);
		++vars->env.alloc_size;
		*envp = tmp_array;
	}
	(*envp)[vars->env.current_size] = NULL;
	(*envp)[vars->env.current_size - 1] = get_var(argv[i + 1]);
	if (!(*envp)[vars->env.current_size - 1])
		return (-1);
	++vars->env.current_size;
	return (1);
}

int	export_var(char **argv, char ***envp, t_vars *vars)
{
	int	status;
	int	return_status;
	int	i;

	if (!argv[1])
		return (solely_export(*envp));
	return_status = 0;
	i = -1;
	while (argv[i + 2])
	{
		++i;
		status = is_valid(&argv[i], &return_status);
		if (status == -1)
			continue ;
		status = already_exists(&argv[i], *envp, status, 0);
		if (status == -1)
			continue ;
		else if (status >= 0)
			return (unset_null(*envp, status, &vars->env.current_size));
		if (add_var(argv, envp, vars, i) == -1)
			return (-1);
	}
	return (return_status);
}
