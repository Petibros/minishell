/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:18:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/15 17:16:26 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **argv, char **envp)
{
	int	i;
	int	status;

	(void) argv;
	status = 0;
	i = 0;
	while (envp && envp[i])
	{
		if (write(1, envp[i], ft_strlen(envp[i])) == -1)
			status = 125;
		write(1, "\n", 1);
		++i;
	}
	return (status);
}
