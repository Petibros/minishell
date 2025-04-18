/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:22:38 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/15 17:15:05 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(char **argv, char **envp)
{
	char	path[PATH_MAX];
	int		status;

	(void) envp;
	status = 0;
	if (argv[1] && argv[1][0] == '-' && argv[1][1])
	{
		write(2, "pwd: no options allowed\n", 24);
		status = 2;
	}
	else if (!getcwd(path, PATH_MAX))
	{
		perror("pwd");
		status = 125;
	}
	else if (write(1, path, ft_strlen(path)) == -1)
	{
		perror("pwd");
		status = 125;
	}
	if (status == 0)
		write(1, "\n", 1);
	return (status);
}
