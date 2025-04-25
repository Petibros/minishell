/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:48:47 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 18:11:52 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

# include "struct.h"

//CLOSE
void	close_pipe(int pipes[2][2], int to_close);
void	close_child_fds(t_vars *vars, int pipes[2][2]);
//FREE
void	exit_and_free(t_vars *vars, int status, bool write_exit);
void	exit_fd_error(t_vars *vars, int pipes[2][2]);
void	exit_no_cmd(t_vars *vars, int pipes[2][2]);
void	exit_error(char *path, char **envp, char **argv, int status);
void	close_fds(t_vars *vars);
void	free_all(t_vars *vars, char **to_not_free, bool in_child);
void	free_branch(t_nodes *tree, char **to_not_free);
void	free_redir(t_redir *node);
void	free_string_array(char **ptr);

#endif
