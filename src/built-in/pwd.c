/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:22:38 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/29 18:43:14 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(char **argv, char **envp)
{
	char	path[PATH_MAX];
	int		status;

	status = 0;
	if (!getcwd(path, PATH_MAX))
	{
		perror("pwd");
		status = 125;
	}
	if (write(1, path, ft_strlen(path)) == -1)
	{
		perror("pwd");
		status = 125;
	}
	write(1, "\n", 1);
	free_string_array(argv);
	free_string_array(envp);
	exit(status);
}
