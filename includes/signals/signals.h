/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 08:02:42 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 08:02:42 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <stdio.h>

void	setup_signals(void);
void	reset_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	ignore_signals(void);

extern int  g_signal_received;

#endif