/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:34:23 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/17 10:24:35 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_line(t_vars *vars)
{
	if (!vars->line[0])
	{
		free(vars->line);
		free(vars->prompt);
		return (0);
	}
	return (1);
}

int	run_shell(t_vars *vars)
{
	vars->sa_setup = &setup_signals;
	actualize_env(vars);
	if (get_prompt(vars) == -1)
		return (0);
	vars->line = readline(vars->prompt);
	if (!vars->line)
		return (0);
	else if (!check_line(vars))
		return (1);
	if (g_signal_received == SIGINT)
	{
		vars->cmd.last_exit_status = 130;
		g_signal_received = 0;
	}
	add_history(vars->line);
	if (parse_line(vars))
		execute(vars, vars->cmd.cmds);
	else
	{
		free_branch(vars->cmd.cmds, NULL);
		vars->cmd.last_exit_status = 2;
	}
	free(vars->line);
	free(vars->prompt);
	return (1);
}

t_vars	*setup_shell(char **envp)
{
	t_vars	*vars;

	setup_signals();
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (NULL);
	vars->cmd.cmds = NULL;
	vars->cmd.last_exit_status = 0;
	vars->cmd.fd_in = 0;
	vars->cmd.fd_out = 1;
	if (transfer_env(envp, vars) == -1)
	{
		free_all(vars, NULL, false);
		return (NULL);
	}
	if (!increment_shlvl(vars))
		perror("malloc error calling \"increment shlvl\"");
	return (vars);
}
