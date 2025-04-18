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

static int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	is_dollar_operator(const char *value)
{
	return (value && value[0] == '$' && 
		(value[1] == '>' || value[1] == '<') && value[2] == '\0');
}

static int	validate_redirection(t_token *next)
{
	if (!next || next->type != TOKEN_WORD)
		return (0);
	if (next->next && next->next->type == TOKEN_LPAREN)
		return (0);
	if (next->type == TOKEN_WORD && is_dollar_operator(next->value))
		return (0);
	return (1);
}

static int	validate_parentheses_content(t_token *current)
{
	// Skip the opening parenthesis
	current = current->next;
	
	// Empty parentheses
	if (!current || current->type == TOKEN_RPAREN)
		return (0);

	// Check for invalid redirections before closing parenthesis
	while (current && current->type != TOKEN_RPAREN)
	{
		if (is_redirection(current->type) && !validate_redirection(current->next))
			return (0);
		if (current->type == TOKEN_WORD && is_dollar_operator(current->value))
			return (0);
		current = current->next;
	}

	// Missing closing parenthesis
	if (!current || current->type != TOKEN_RPAREN)
		return (0);

	return (1);
}

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
		if (is_redirection(current->type) && !validate_redirection(next))
			return (0);
		if (current->type == TOKEN_LPAREN && !validate_parentheses_content(current))
			return (0);
		if (current->type == TOKEN_WORD && is_dollar_operator(current->value))
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
		return (1);
	if (!validate_syntax(tokens))
	{
		free_token(tokens);
		vars->cmd.last_exit_status = 2;  // Set syntax error status
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
