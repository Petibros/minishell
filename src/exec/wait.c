/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:35:17 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 17:35:17 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_processes(int last_known_exit_status, int last_known_pid)
{
	int	status;
	int	pid;
	int	last_pid;
	int	last_status;

	last_pid = 0;
	last_status = -1;
	pid = waitpid(-1, &status, 0);
	while (pid != -1)
	{
		if (pid > last_pid)
		{
			last_pid = pid;
			last_status = status;
		}
		pid = waitpid(-1, &status, 0);
	}
	if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGINT)
		return (130);
	else if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGQUIT)
		return (print_quit());
	if (last_status == -1 || !last_known_pid)
		return (last_known_exit_status);
	return (WEXITSTATUS(last_status));
}
