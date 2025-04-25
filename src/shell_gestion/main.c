/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 18:17:56 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:20:59 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;
	int		status;

	(void) argv;
	if (argc > 1 || !envp)
	{
		if (argc > 1)
			write(2, "No arguments needed\n", 20);
		return (1);
	}
	vars = setup_shell(envp);
	if (!vars)
		return (1);
	while (true)
	{
		if (!run_shell(vars))
			break ;
	}
	rl_clear_history();
	status = vars->cmd.last_exit_status;
	exit_and_free(vars, status, true);
	return (status);
}
