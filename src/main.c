/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/19 17:16:58 by sacgarci         ###   ########.fr       */
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
//c'est toi le pointeur baaaaaaaaaaaakaaaaaaaa

	(void) argv;
	if (argc > 1 || !envp)
		return (1);
	vars = malloc(sizeof(t_vars));
	if (!vars)
		return (1);
	if (transfer_env(envp, vars) == -1)//les variables d'environnement sont malloc pour être modifiées dynamiquement
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
		if (ft_strncmp(vars->line, "exit", 5) == 0)
		{
			print_exit();
			free(vars->line);
			free(vars->prompt);
			break ;
		}
		if (ft_strncmp(vars->line, "cd", 3) == 0 || ft_strncmp(vars->line, "cd ", 3) == 0)
		{
			if (*(vars->line + 2) == 0 || *(vars->line + 3) == 0)
				cd(vars->home_path);
			else
				cd(vars->line + 3);
		}
		if (ft_strncmp(vars->line, "pwd", 4) == 0)
			pwd(vars);
		if (ft_strncmp(vars->line, "env", 5) == 0)
			env(vars->env.envp);
		if (ft_strncmp(vars->line, "unset ", 6) == 0)
			unset(vars->line + 6, vars);
		if (ft_strncmp(vars->line, "export", 7) == 0)
			if (export_var("CACA=", "cat", vars) == -1)
				break ;
		if (ft_strncmp(vars->line, "echo -n ", 8) == 0)
			echo(vars->line + 8, 1, true);
		else if (ft_strncmp(vars->line, "echo ", 5) == 0)
			echo(vars->line + 5, 1, false);
		if (ft_strncmp(vars->line, "here_doc ", 9) == 0)
			here_doc(2, vars->line + 9);
		// Parsing renvoie 1 si tout va bien 0 si ça a foiré
		// Pas besoin de reset cmds entre les cycles de line (mais besoin en dehors du while)
		parse_line(vars); 
		free(vars->line);
		free(vars->prompt);
	}
	rl_clear_history();
	free_vars(vars);
}
