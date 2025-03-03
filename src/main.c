/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/04 00:10:01 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_prompt(t_vars *vars)
{
	char	*tmp;

	if (ft_strnstr(vars->abs_path, vars->home_path,
			ft_strlen(vars->home_path)) == vars->abs_path)
	{
		vars->prompt = ft_strjoin(vars->user, " : ~");
		tmp = ft_strjoin(vars->prompt,
				vars->abs_path + ft_strlen(vars->home_path));
		free(vars->prompt);
		vars->prompt = ft_strjoin(tmp, " > ");
		free(tmp);
	}
	else
	{
		vars->prompt = ft_strjoin(vars->user, " : ");
		tmp = ft_strjoin(vars->prompt, vars->abs_path);
		free(vars->prompt);
		vars->prompt = ft_strjoin(tmp, " > ");
		free(tmp);
	}
	return (0);
}

int	actualize_env(t_vars *vars)
{
	vars->user = getenv("USER");
	vars->home_path = getenv("HOME");
	getcwd(vars->abs_path, PATH_MAX);
	get_prompt(vars);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;
	int		n = 0;

	(void) argv;
	if (argc > 1 || !envp)
		return (1);
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (1);
	while (n < 5)
	{
		++n;
		actualize_env(vars);
		vars->input.line = readline(vars->prompt);
		free(vars->input.line);
		free(vars->prompt);
	}
	free(vars);
}
