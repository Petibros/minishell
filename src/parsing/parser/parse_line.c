/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:55:37 by npapash           #+#    #+#             */
/*   Updated: 2025/04/18 15:21:58 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	validate_syntax(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return (0);
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (0);
	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_PIPE && (!next || next->type == TOKEN_PIPE))
			return (0);
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_APPEND || current->type == TOKEN_HEREDOC)
		{
			if (!next || next->type != TOKEN_WORD || 
				(next->next && next->next->type == TOKEN_LPAREN))
				return (0);
		}
		current = next;
	}
	return (1);
}

int	parse_line(t_vars *vars)
{
	t_token	*tokens;

	if (!vars->line || !*vars->line)
		return (0);
	tokens = lexer(vars->line);
	if (!tokens)
		return (1);
	if (!validate_syntax(tokens))
	{
		free_token(tokens);
		return (0);
	}
	expand_variables_in_tokens(&tokens, vars->cmd.last_exit_status,
		vars->env.envp);
	vars->cmd.cmds = pratt_parse(tokens, vars->env.envp, vars);
	if (!vars->cmd.cmds)
	{
		free_token(tokens);
		if (vars->cmd.last_exit_status == 2)  // Check if error was due to syntax error
			return (0);
		return (1);
	}
	expand_wildcards(vars->cmd.cmds);
	handle_quotes_in_node(vars->cmd.cmds);
	//print_ast(vars->cmd.cmds);  // Print the AST after all processing is done
	free_token(tokens);
	return (1);
}
