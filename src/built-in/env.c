/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:18:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/18 14:42:32 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **argv, char **envp)
{
	int	i;
	int	j;
	int	status;

	(void) argv;
	status = 0;
	i = 0;
	while (envp && envp[i])
	{
		j = 0;
		while (envp[i][j] && (ft_isalnum(envp[i][j]) || envp[i][j] == '_')
			&& !ft_isdigit(envp[i][0]))
			++j;
		if (envp[i][j] == '=')
		{
			if (write(1, envp[i], ft_strlen(envp[i])) == -1)
				status = 125;
			write(1, "\n", 1);
		}
		++i;
	}
	return (status);
}
