/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:55:37 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 17:55:37 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

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
	printf("\n\033[1;34mAST:\033[0m");
	print_ast(vars->cmd.cmds);
	free_node(vars->cmd.cmds);
	vars->cmd.cmds = NULL;
	free_token(tokens);
	return (1);
}
