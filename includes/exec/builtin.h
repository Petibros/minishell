/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:50:16 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/25 18:00:38 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "struct.h"

int		solely_export(char **envp);
int		unset_null(char **envp, int i, int *current_size);
int		echo(char **argv, char **envp);
int		export_var(char **argv, char ***envp, t_vars *vars);
int		cd(char **argv, t_vars *vars);
int		pwd(char **argv, char **envp);
int		unset(char **argv, t_vars *vars);
int		env(char **argv, char **envp);
int		exit_built_in(char **argv, t_vars *vars, bool write_exit);
char	*get_var(char *argv);

#endif
