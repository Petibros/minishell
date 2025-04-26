/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 08:08:19 by npapash           #+#    #+#             */
/*   Updated: 2025/04/26 18:09:40 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	if (waitpid(-1, NULL, WNOHANG) == -1)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_sigint_subshell(int sig)
{
	(void)sig;
	g_signal_received = SIGINT;
}

void	handle_sigint_heredoc(int sig)
{
	(void) sig;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	close(0);
}
