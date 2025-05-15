/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:50:43 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:50:43 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_nodes	*create_subshell_node(t_nodes *inner_cmd)
{
	t_nodes	*node;

	node = create_node();
	if (!node)
	{
		free_node(inner_cmd);
		return (NULL);
	}
	node->is_operator = 1;
	node->operator_type = TOKEN_SUBSHELL;
	node->left = inner_cmd;
	return (node);
}

static int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	is_valid_token_after_subshell(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC
		|| type == TOKEN_EOF || type == TOKEN_RPAREN);
}

static t_nodes	*handle_redirections_wrapper(t_nodes *node, t_token **token)
{
	if (*token && is_redirection_token((*token)->type)
		&& !handle_redirections(node, token))
	{
		free_node(node);
		return (NULL);
	}
	if (*token && !is_valid_token_after_subshell((*token)->type))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd((*token)->value, 2);
		ft_putstr_fd("'\n", 2);
		free_node(node);
		return (NULL);
	}
	return (node);
}

t_nodes	*finalize_subshell(t_nodes *inner_cmd, t_token **token)
{
	t_nodes	*node;

	node = create_subshell_node(inner_cmd);
	if (!node)
		return (NULL);
	return (handle_redirections_wrapper(node, token));
}
