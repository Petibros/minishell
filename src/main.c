/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/06 18:57:03 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;
	int		status;

	(void) argv;
	if (argc > 1 || !envp)
		return (1);
	setup_signals();
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (1);
	vars->cmd.cmds = NULL;
	vars->cmd.last_exit_status = 0;
	if (transfer_env(envp, vars) == -1)
	{
		free_all(vars, NULL, false);
		return (1);
	}
	while (true)
	{
		actualize_env(vars);
		if (get_prompt(vars) == -1)
			break ;
		vars->line = readline(vars->prompt);
		if (!vars->line)
			break ;
		if (!vars->line[0])
			continue ;
		add_history(vars->line);
		parse_line(vars);
		execute(vars, vars->cmd.cmds);
		free(vars->line);
		free(vars->prompt);
	}
	rl_clear_history();
	status = vars->cmd.last_exit_status;
	exit_and_free(vars, status);
	return (status);
}
