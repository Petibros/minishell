/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_utils3.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 11:50:43 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:50:43 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRATT_UTILS3_H
# define PRATT_UTILS3_H

# include "parsing.h"

t_nodes	*finalize_subshell(t_nodes *inner_cmd, t_token **token, t_vars *vars);

#endif
