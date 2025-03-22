/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/14 16:41:05 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line(t_vars *vars)
{
	t_token *tokens;

	if (!vars->input.line || !*vars->input.line)
		return (0);
	tokens = lexer(vars->input.line);
	if (!tokens)
		return (0);
	vars->cmd.cmds = pratt_parse(tokens);
	if (!vars->cmd.cmds)
	{
		free_token(tokens);
		return (0);
	}
	expand_variables_in_node(vars->cmd.cmds, vars->cmd.last_exit_status);
	expand_wildcards(vars->cmd.cmds);
	handle_quotes_in_node(vars->cmd.cmds);
	// Print only the final AST
	printf("\n\033[1;34mAST:\033[0m");
	print_ast(vars->cmd.cmds);
	free_node(vars->cmd.cmds);
	vars->cmd.cmds = NULL;
	free_token(tokens);
	return (1);
}

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
		vars->input.line = readline(vars->prompt);
		add_history(vars->input.line);
		if (ft_strncmp(vars->input.line, "exit", 5) == 0)
		{
			print_exit();
			free(vars->input.line);
			free(vars->prompt);
			break ;
		}
		if (ft_strncmp(vars->input.line, "cd", 3) == 0 || ft_strncmp(vars->input.line, "cd ", 3) == 0)
		{
			if (*(vars->input.line + 2) == 0 || *(vars->input.line + 3) == 0)
				cd(vars->home_path);
			else
				cd(vars->input.line + 3);
		}
		if (ft_strncmp(vars->input.line, "pwd", 4) == 0)
			pwd(vars);
		if (ft_strncmp(vars->input.line, "env", 5) == 0)
			env(vars->env.envp);
		if (ft_strncmp(vars->input.line, "unset ", 6) == 0)
			unset(vars->input.line + 6, vars);
		if (ft_strncmp(vars->input.line, "export", 7) == 0)
			if (export_var("CACA=", "cat", vars) == -1)
				break ;
		if (ft_strncmp(vars->input.line, "echo -n ", 8) == 0)
			echo(vars->input.line + 8, 1, true);
		else if (ft_strncmp(vars->input.line, "echo ", 5) == 0)
			echo(vars->input.line + 5, 1, false);
		if (ft_strncmp(vars->input.line, "here_doc ", 9) == 0)
			here_doc(2, vars->input.line + 9);
		if (parse_line(vars) == -1)
			break;
		free(vars->input.line);
		free(vars->prompt);
	}
	rl_clear_history();
	free_vars(vars);
}