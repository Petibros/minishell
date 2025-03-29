#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include "../minishell.h"

void    setup_signals(void);
void    reset_signals(void);
void    handle_sigint(int sig);
void    handle_sigquit(int sig);
void    ignore_signals(void);

extern int g_signal_received;

#endif
