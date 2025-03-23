/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:20:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:20:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODES_H
# define NODES_H

# include "types.h"

/* Node creation functions */
t_nodes		*create_node(void);
t_nodes		*create_op_node(t_nodes *left, t_nodes *right, t_token_type op_type);
t_nodes		*create_cmd_node(t_token **token, int argc);

#endif
