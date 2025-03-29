/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_node_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 07:22:14 by npapash           #+#    #+#             */
/*   Updated: 2025/03/29 07:22:14 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRATT_NODE_UTILS_H
# define PRATT_NODE_UTILS_H

# include "types.h"

char		*get_operator_string(t_token_type op_type);
t_nodes		*init_op_node(t_nodes *node, t_nodes *left,
				t_nodes *right, t_token_type op_type);
t_nodes		*init_cmd_argv(t_nodes *node, t_token *start, int argc);

#endif
