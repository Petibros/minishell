/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:34:42 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:34:43 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0
			&& envp[i][ft_strlen(var)] == '=')
			return (envp[i] + ft_strlen(var) + 1);
		++i;
	}
	return (NULL);
}

void	swap_str(char **str_1, char **str_2)
{
	char	*tmp;

	tmp = *str_1;
	*str_1 = *str_2;
	*str_2 = tmp;
}

void	sort_env(char ***envp)
{
	int		i;
	int		limit;
	int		j;

	limit = 0;
	while ((*envp)[limit + 1])
	{
		i = 0;
		while ((*envp)[i + limit] && (*envp)[i + limit + 1])
		{
			j = 0;
			while ((*envp)[i][j])
			{
				if ((*envp)[i][j] < (*envp)[i + 1][j])
					swap_str(&(*envp)[i], &(*envp)[i + 1]);
				if ((*envp)[i][j] != (*envp)[i + 1][j])
					break ;
				++j;
			}
			++i;
		}
		++limit;
	}
}

int	transfer_env(char **envp, t_vars *vars)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		++i;
	vars->env.current_size = i + 1;
	vars->env.alloc_size = i + 1;
	vars->env.envp = malloc((vars->env.alloc_size) * sizeof(char *));
	if (!vars->env.envp)
		return (-1);
	i = 0;
	while (envp && envp[i])
	{
		vars->env.envp[i] = ft_strdup(envp[i]);
		if (!vars->env.envp[i])
			return (-1);
		++i;
	}
	vars->env.envp[i] = NULL;
	if (vars->env.envp[0])
		sort_env(&vars->env.envp);
	return (1);
}

void	actualize_env(t_vars *vars)
{
	vars->user = ft_getenv(vars->env.envp, "USER");
	vars->home_path = ft_getenv(vars->env.envp, "HOME");
	getcwd(vars->abs_path, PATH_MAX);
}
