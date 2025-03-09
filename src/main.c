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

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;
	t_cmds	cmds; // Pas de pointer pour pas avoir besoin de malloc

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
			export_var("CACA=", "cat", vars);
		// Parsing renvoie 1 si tout va bien 0 si ça a foiré
		// Pas besoin de reset cmds entre les cycles de line (mais besoin en dehors du while)
		parse_line(vars->input.line, envp, &cmds); 
		free(vars->input.line);
		free(vars->prompt);
	}
	rl_clear_history();
	free(vars);
}
