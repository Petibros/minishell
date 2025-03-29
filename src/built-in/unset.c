/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:04:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/27 23:46:04 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_unset(char **argv, char *env)
{
	int	i;
	int	var_len;

	i = 0;
	while (argv[i])
	{
		var_len = ft_strlen(argv[i]);
		if (ft_strncmp(argv[i], env, var_len) == 0 && *(env + var_len) == '=')
			return (1);
		++i;
	}
	return (0);
}

void	unset(char **argv, t_vars *vars)
{
	int	i;
	int	tmp_i;

	i = 0;
	while (vars->env.envp[i])
	{
		if (is_unset(argv, vars->env.envp[i]))
		{
			tmp_i = i;
			while (vars->env.envp[tmp_i])
			{
				vars->env.envp[tmp_i] = vars->env.envp[tmp_i + 1];
				++tmp_i;
			}
			--vars->env.current_size;
		}
		++i;
	}
}
