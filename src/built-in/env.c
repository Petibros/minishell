/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:18:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/29 19:03:25 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(char **argv, char **envp)
{
	int	i;
	int	status;

	status = 0;
	i = 0;
	while (envp && envp[i])
	{
		if (write(1, envp[i], ft_strlen(envp[i])) == -1)
			status = 125;
		write(1, "\n", 1);
		++i;
	}
	free_string_array(argv);
	free_string_array(envp);
	exit(status);
}
