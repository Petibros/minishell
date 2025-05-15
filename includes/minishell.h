/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:51:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/14 23:53:03 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define OR 1
# define AND 2
# define PIPE 3

# include <linux/limits.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <termios.h>
# include "parsing/types.h"
# include "libft.h"
# include "parsing.h"
# include "signals.h"
# include "free.h"
# include "exec.h"
# include "struct.h"
# include "builtin.h"

//PARSING
int		parse_line(t_vars *vars);
//EXECUTION
void	execute(t_vars *vars, t_nodes *cmds);
//ENVIRONMENT
int		transfer_env(char **envp, t_vars *vars);
void	actualize_env(t_vars *vars);
char	*ft_getenv(char **envp, char *var);
//SHELL_GESTION
t_vars	*setup_shell(char **envp);
int		run_shell(t_vars *vars);
void	swap_str(char **str_1, char **str_2);
int		increment_shlvl(t_vars *vars);
//READLINE PROMPT
int		get_prompt(t_vars *vars);

//Exec parse
void	new_expand_variables_in_node(t_nodes *node, t_vars *vars);
void	new_expand_wildcards_in_node(t_nodes *node, t_vars *vars, int *status);
void	new_remove_quotes(char *str);
void	remove_all_quotes(t_nodes *node);

#endif
