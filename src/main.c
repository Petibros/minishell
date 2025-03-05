/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/06 00:26:10 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	transfer_env(char **envp, t_vars *vars)
{
	int	i;

	i = 0;
	while (envp[i])
		++i;
	vars->env.current_size = i;
	vars->env.alloc_size = i;
	vars->env.envp = malloc(vars->env.alloc_size * sizeof(char *) + 1);
	i = 0;
	while (envp[i])
	{
		vars->env.envp[i] = ft_strdup(envp[i]);
		++i;
	}
	vars->env.envp[i] = NULL;
}

static int	actualize_env(t_vars *vars)
{
	vars->user = getenv("USER");
	vars->home_path = getenv("HOME");
	getcwd(vars->abs_path, PATH_MAX);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;

	(void) argv;
	if (argc > 1 || !envp)
		return (1);
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (1);
	transfer_env(envp, vars);
	envp = vars->env.envp;
	while (true)
	{
		actualize_env(vars);
		if (get_prompt(vars) == -1)
			break ;
		vars->input.line = readline(vars->prompt);
		add_history(vars->input.line);
		if (ft_strncmp(vars->input.line, "exit", 5) == 0)
		{
			print_exit();
			free(vars->input.line);
			free(vars->prompt);
			break ;
		}
		if (ft_strncmp(vars->input.line, "cd ", 3) == 0)
			cd(vars->input.line + 3);
		if (ft_strncmp(vars->input.line, "pwd", 4) == 0)
			pwd(vars);
		if (ft_strncmp(vars->input.line, "env", 5) == 0)
			env(vars->env.envp);
		if (ft_strncmp(vars->input.line, "unset ", 6) == 0)
			unset(vars->input.line + 6, vars);
		if (ft_strncmp(vars->input.line, "export", 7) == 0)
			export_var(envp, "CACA=", "cat", vars);
		free(vars->input.line);
		free(vars->prompt);
	}
	rl_clear_history();
	free(vars);
}
