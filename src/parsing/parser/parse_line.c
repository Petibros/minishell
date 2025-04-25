/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 17:55:37 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:27:42 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static char	*get_error_token(t_token *tokens)
{
	t_token *current;

	current = tokens;
	while (current && current->next)
	{
		if ((current->next->type == TOKEN_PIPE || current->next->type == TOKEN_AND 
			|| current->next->type == TOKEN_OR) && !current->next->next)
			return (current->next->value);
		current = current->next;
	}
	if (current && (current->type == TOKEN_PIPE || current->type == TOKEN_AND 
		|| current->type == TOKEN_OR))
		return (current->value);
	return (NULL);
}

static int	handle_syntax_error(t_token *tokens, t_vars *vars)
{
	print_syntax_error(get_error_token(tokens));
	free_token(tokens);
	vars->cmd.last_exit_status = 2;
	return (0);
}

static int	handle_parse_error(t_token *tokens, t_vars *vars)
{
	free_token(tokens);
	if (vars->cmd.last_exit_status == 2)
		return (0);
	return (1);
}

static int	process_tokens(t_token *tokens, t_vars *vars)
{
	expand_variables_in_tokens(&tokens, vars->cmd.last_exit_status,
		vars->env.envp);
	vars->cmd.cmds = pratt_parse(tokens, vars->env.envp, vars);
	if (!vars->cmd.cmds)
		return (handle_parse_error(tokens, vars));
	expand_wildcards(vars->cmd.cmds);
	handle_quotes_in_node(vars->cmd.cmds);
	free_token(tokens);
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
		return (handle_syntax_error(tokens, vars));
	return (process_tokens(tokens, vars));
}
