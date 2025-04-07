/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 08:08:19 by npapash           #+#    #+#             */
/*   Updated: 2025/04/07 06:43:07 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

/* handle ctrl-c */
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
		g_signal_received = 0;
	}
}

void	handle_sigint_heredoc(int sig)
{
	(void) sig;
	g_signal_received = SIGINT;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	close(0);
}
