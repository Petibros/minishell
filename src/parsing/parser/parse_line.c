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
#include "print_ast.h"

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
		if ((current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
				|| current->type == TOKEN_APPEND
				|| current->type == TOKEN_HEREDOC) && (!next
				|| next->type != TOKEN_WORD))
			return (0);
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
		return (0);
	if (!validate_syntax(tokens))
	{
		free_token(tokens);
		return (0);
	}
	expand_variables_in_tokens(&tokens, vars->cmd.last_exit_status, vars->env.envp);
	vars->cmd.cmds = pratt_parse(tokens, vars->env.envp);
	if (!vars->cmd.cmds)
	{
		free_token(tokens);
		return (0);
	}
	expand_wildcards(vars->cmd.cmds);
	handle_quotes_in_node(vars->cmd.cmds);
	free_token(tokens);
	return (1);
}
