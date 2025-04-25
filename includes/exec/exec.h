/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:40:00 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 18:06:11 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "struct.h"

//FILE_DESCRIPTORS
int		get_fds(t_vars *vars, t_nodes *cmds, int is_pipe[2]);
void	init_pipes(int pipes[2][2]);
//HEREDOC
void	heredoc_gestion(t_vars *vars, t_redir *files, int *fd);
//EXECUTION
int		is_exec(char *path, int *status);
int		print_quit(void);
char	*get_path(char *cmd, char **envp, int *status);
void	execute(t_vars *vars, t_nodes *cmds);
int		exec_routine(t_vars *vars, t_nodes *cmds, int is_pipe[2]);
void	exec_cmd(t_vars *vars, t_nodes *cmds);
int		wait_processes(int last_known_exit_status, int last_known_pid);

#endif
