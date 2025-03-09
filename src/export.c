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

static char	**alloc_new_array(t_vars *vars, int size)
{
	int		i;
	char	**new_array;

	i = 0;
	new_array = malloc(size * sizeof(char *) + 1);
	while (vars->env.envp[i])
	{
		new_array[i] = ft_strdup(vars->env.envp[i]);
		++i;
	}
	new_array[i] = NULL;
	free(vars->env.envp);
	return (new_array);
}

void	export_var(char **envp, char *var, char *value, t_vars *vars)
{
	int	i;

	i = 0;
	if (vars->env.current_size >= vars->env.alloc_size)
	{
		vars->env.alloc_size += 8;
		vars->env.envp = alloc_new_array(vars, vars->env.alloc_size);
		envp = vars->env.envp;
	}
	vars->env.envp[vars->env.current_size + 1] = NULL;
	vars->env.envp[vars->env.current_size] = ft_strjoin(var, value);
}
