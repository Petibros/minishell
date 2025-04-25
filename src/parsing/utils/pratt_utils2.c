/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:46:56 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:50:43 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "pratt_utils3.h"

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static t_nodes	*handle_empty_parentheses(t_token **token, t_vars *vars)
{
	*token = (*token)->next;
	vars->cmd.last_exit_status = 2;
	return (NULL);
}

static t_nodes	*handle_inner_expression(t_token **token, char **envp,
	t_vars *vars)
{
	t_nodes	*inner_cmd;

	inner_cmd = parse_expression(token, 0, envp, vars);
	if (!inner_cmd)
		return (NULL);
	if (!*token || (*token)->type != TOKEN_RPAREN)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	*token = (*token)->next;
	return (inner_cmd);
}

t_nodes	*handle_parentheses(t_token **token, char **envp, t_vars *vars)
{
	t_nodes	*inner_cmd;

	if (!*token || (*token)->type != TOKEN_LPAREN)
		return (NULL);
	*token = (*token)->next;
	if (*token && (*token)->type == TOKEN_RPAREN)
		return (handle_empty_parentheses(token, vars));
	inner_cmd = handle_inner_expression(token, envp, vars);
	if (!inner_cmd)
		return (NULL);
	return (finalize_subshell(inner_cmd, token, envp));
}
