/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigquit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 08:08:19 by npapash           #+#    #+#             */
/*   Updated: 2025/04/24 18:51:41 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("\nsalut\n");
	write(1, "Quit\n\n", 5);
	exit(131);
}
