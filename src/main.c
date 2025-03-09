/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/05 17:20:20 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	actualize_env(t_vars *vars)
{
	vars->user = getenv("USER");
	vars->home_path = getenv("HOME");
	getcwd(vars->abs_path, PATH_MAX);
	return (0);
}

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
	while (true)
	{
		actualize_env(vars);
		if (get_prompt(vars) == -1)
			break ;
		vars->input.line = readline(vars->prompt);
		// Parsing renvoie 1 si tout va bien 0 si ça a foiré
		// Pas besoin de reset cmds entre les cycles de line (mais besoin en dehors du while)
		parse_line(vars->input.line, envp, &cmds); 
		free(vars->input.line);
		free(vars->prompt);
	}
	free(vars);
}
