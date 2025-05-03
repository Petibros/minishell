/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:50:44 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:17:17 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	actualize_env_last_cmd(t_vars *vars, t_nodes *cmds)
{
	char	**to_export;
	int		status;

	to_export = malloc(3 * sizeof(char *));
	if (!to_export)
		return (-1);
	to_export[0] = "export";
	if (cmds->argv && cmds->argv[0])
		to_export[1] = ft_strjoin("_=", cmds->argv[0]);
	else
		to_export[1] = ft_strjoin("_=", "");
	if (!to_export[1])
	{
		free(to_export);
		return (-1);
	}
	to_export[2] = NULL;
	status = export_var(to_export, &vars->env.envp, vars);
	free(to_export[1]);
	free(to_export);
	return (status);
}

static int	which_built_in(char **argv)
{
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (1);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (2);
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (3);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (4);
	return (0);
}

static int	is_built_in(t_vars *vars, t_nodes *cmds, int is_pipe[2])
{
	int	status;

	status = which_built_in(cmds->argv);
	if (!status || is_pipe[0] || is_pipe[1])
		return (-2);
	new_expand_variables_in_node(cmds, vars->env.envp, vars);
	if (status == 1)
		status = export_var(cmds->argv, &vars->env.envp, vars);
	else if (status == 2)
		status = unset(cmds->argv, vars);
	else if (status == 3)
		status = cd(cmds->argv, vars);
	else if (status == 4)
		status = exit_built_in(cmds->argv, vars, true);
	if (status != -1)
		vars->cmd.last_exit_status = status;
	return (status);
}

static int	fork_routine(t_vars *vars, t_nodes *cmds)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("error during fork");
		return (-1);
	}
	if (pid == 0)
	{
		reset_signals();
		exec_cmd(vars, cmds);
	}
	vars->cmd.last_pid = pid;
	return (0);
}

int	exec_routine(t_vars *vars, t_nodes *cmds, int is_pipe[2])
{
	int	status;

	status = 0;
	vars->cmd.last_pid = 0;
	if (actualize_env_last_cmd(vars, cmds) == -1)
		return (-1);
	
	status = get_fds(vars, cmds, is_pipe);
	if (status != 0)
		return (status);
	if (!cmds->argv || !cmds->argv[0])
	{
		close_fds(vars);
		vars->cmd.last_exit_status = 0;
		return (0);
	}
	status = is_built_in(vars, cmds, is_pipe);
	if (status == -1)
		perror("malloc error");
	else if (status == -2)
		status = fork_routine(vars, cmds);
	return (status);
}
