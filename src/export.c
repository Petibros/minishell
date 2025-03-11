/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:48:28 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/06 00:15:56 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	already_exists(char *var, char *value, t_vars *vars)
{
	int		i;

	i = 0;
	while (vars->env.envp[i])
	{
		if (ft_strncmp(var, vars->env.envp[i], ft_strlen(var)) == 0)
		{
			free(vars->env.envp[i]);
			vars->env.envp[i] = ft_strjoin(var, value);
			if (!vars->env.envp[i])
			{
				++i;
				while (vars->env.envp[i])
				{
					free(vars->env.envp[i]);
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

static char	**alloc_new_array(t_vars *vars, int size)
{
	int		i;
	char	**new_array;

	i = 0;
	new_array = malloc((size + 1) * sizeof(char *));
	while (new_array && vars->env.envp[i])
	{
		new_array[i] = ft_strdup(vars->env.envp[i]);
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
	i = 0;
	while (vars->env.envp[i])
	{
		free(vars->env.envp[i]);
		vars->env.envp[i] = NULL;
		++i;
	}
	free(vars->env.envp);
	return (new_array);
}

int	export_var(char *var, char *value, t_vars *vars)
{
	int	status;

	status = already_exists(var, value, vars);
	if (status != 0)
		return (status);
	if (vars->env.current_size >= vars->env.alloc_size)
	{
		++vars->env.alloc_size;
		vars->env.envp = alloc_new_array(vars, vars->env.alloc_size);
		if (!vars->env.envp)
			return (-1);
	}
	vars->env.envp[vars->env.current_size + 1] = NULL;
	vars->env.envp[vars->env.current_size] = ft_strjoin(var, value);
	if (!vars->env.envp[vars->env.current_size])
		return (-1);
	++vars->env.current_size;
	return (1);
}
