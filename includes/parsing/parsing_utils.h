/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:21:45 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 04:21:45 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_UTILS_H
# define PARSING_UTILS_H

# include "parsing.h"

/* parsing_utils2.c */
void	print_node_recursive(t_nodes *node, int depth);
void	print_ast(t_nodes *root);

/* parsing_utils3.c */
void	print_redirections(t_nodes *node, int depth);

#endif
