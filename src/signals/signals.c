/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 08:06:22 by npapash           #+#    #+#             */
/*   Updated: 2025/04/24 19:53:11 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

int	g_signal_received = 0;

void	setup_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint_heredoc;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}

void	setup_signals_subshell(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint_subshell;
	sa_int.sa_flags = SA_RESTART;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
