/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:11:38 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 03:11:38 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_nodes	*parse(t_token *token, char **envp, t_vars *vars)
{
	t_nodes	*ast;

	if (!token)
		return (NULL);
	ast = parse_and_or(&token, envp, vars);
	if (!ast || token)
	{
		free_node(ast);
		return (NULL);
	}
	return (ast);
}
