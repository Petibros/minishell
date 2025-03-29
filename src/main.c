/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/27 23:37:41 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	free_vars(t_vars *vars)
{
	int	i;

	if (vars->env.envp)
	{
		i = 0;
		while (vars->env.envp[i])
		{
			free(vars->env.envp[i]);
			++i;
		}
		free(vars->env.envp);
	}
	free(vars);
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
	if (transfer_env(envp, vars) == -1)
	{
		free_vars(vars);
		return (1);
	}
	while (true)
	{
		actualize_env(vars);
		if (get_prompt(vars) == -1)
			break ;
		vars->line = readline(vars->prompt);
		add_history(vars->line);
		parse_line(vars);
		execute(vars, vars->cmd.cmds);
		free(vars->line);
		free(vars->prompt);
	}
	rl_clear_history();
	free_vars(vars);
}
