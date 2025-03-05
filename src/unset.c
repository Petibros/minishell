/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:04:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/05 23:49:22 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(char *var, t_vars *vars)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (vars->env.envp[i])
	{
		if (ft_strncmp(vars->env.envp[i], var, var_len) == 0
			&& vars->env.envp[i][var_len] == '=')
		{
			while (vars->env.envp[i])
			{
				vars->env.envp[i] = vars->env.envp[i + 1];
				++i;
			}
			--vars->env.current_size;
			return ;
		}
		++i;
	}
}
