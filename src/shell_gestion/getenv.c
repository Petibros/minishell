/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:34:42 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/13 17:50:40 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char **envp, char *var)
{
	int	i;

	if (!var)
		return (NULL);
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

static int	fill_argv_shlvl(char *var_ptr, int i, char **argv)
{
	char	*nb;

	argv[0] = "export";
	if (!var_ptr || var_ptr[i])
		argv[1] = ft_strdup("SHLVL=0");
	else
	{
		nb = ft_itoa(ft_atoi(var_ptr) + 1);
		argv[1] = ft_strjoin("SHLVL=", nb);
		if (nb)
			free(nb);
	}
	argv[2] = NULL;
	if (!argv[1])
	{
		free(argv);
		return (0);
	}
	return (1);
}

int	increment_shlvl(t_vars *vars)
{
	char	*var_ptr;
	char	**argv;
	int		status;
	int		i;

	i = 0;
	argv = malloc(3 * sizeof(char *));
	if (!argv)
		return (0);
	var_ptr = ft_getenv(vars->env.envp, "SHLVL");
	while (var_ptr && ft_isdigit(var_ptr[i]))
		++i;
	if (!fill_argv_shlvl(var_ptr, i, argv))
		return (0);
	status = export_var(argv, &vars->env.envp, vars);
	free(argv[1]);
	free(argv);
	if (status == -1)
		return (0);
	return (1);
}

static void	sort_env(char ***envp)
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
