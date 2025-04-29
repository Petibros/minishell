/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:32:49 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/28 20:33:59 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	actualize_env(t_vars *vars)
{
	vars->user = ft_getenv(vars->env.envp, "USER");
	vars->home_path = ft_getenv(vars->env.envp, "HOME");
	getcwd(vars->abs_path, PATH_MAX);
}

void	swap_str(char **str_1, char **str_2)
{
	char	*tmp;

	tmp = *str_1;
	*str_1 = *str_2;
	*str_2 = tmp;
}
