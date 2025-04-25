/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:02:42 by npapash           #+#    #+#             */
/*   Updated: 2025/04/24 19:54:14 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <stdio.h>

extern int	g_signal_received;

void	setup_signals(void);
void	setup_signals_subshell(void);
void	setup_signals_heredoc(void);
void	reset_signals(void);
void	handle_sigint(int sig);
void	handle_sigint_heredoc(int sig);
void	handle_sigint_subshell(int sig);

#endif
