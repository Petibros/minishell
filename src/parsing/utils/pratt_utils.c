/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pratt_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <npapash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:11:38 by npapash           #+#    #+#             */
/*   Updated: 2025/04/25 11:46:56 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	get_precedence(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (3);
	if (type == TOKEN_AND || type == TOKEN_OR)
		return (1);
	return (0);
}
