/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:04:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/06/07 16:31:41 by npapashv         ###   ########.fr       */
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
		if (ft_strncmp(argv[i], env, var_len) == 0
			&& (*(env + var_len) == '=' || !*(env + var_len)))
			return (1);
		++i;
	}
	return (0);
}

int	unset(char **argv, t_vars *vars)
{
	int	i;
	int	tmp_i;

	i = 0;
	while (vars->env.envp[i])
	{
		if (is_unset(argv, vars->env.envp[i]))
		{
			tmp_i = i;
			free(vars->env.envp[tmp_i]);
			vars->env.envp[tmp_i] = NULL;
			while (vars->env.envp[tmp_i + 1])
			{
				vars->env.envp[tmp_i] = vars->env.envp[tmp_i + 1];
				++tmp_i;
			}
			vars->env.envp[tmp_i] = NULL;
			--vars->env.current_size;
			continue ;
		}
		++i;
	}
	return (0);
}
