/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:18:12 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 03:18:12 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_nodes	*parse(t_token *token, char **envp)
{
	t_nodes	*ast;

	ast = parse_and_or(&token, envp);
	if (!ast || (token && token->type != TOKEN_EOF))
	{
		free_node(ast);
		return (NULL);
	}
	return (ast);
}
