/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:51:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/20 00:08:31 by sacgarci         ###   ########.fr       */
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
# include "parsing.h"
# include "libft.h"

typedef struct s_env
{
	int		current_size;
	int		alloc_size;
	char	**envp;
}	t_env;

typedef struct s_vars
{
	char	*line;
	t_env	env;
	t_cmds	cmd;
	char	abs_path[PATH_MAX];
	char	*home_path;
	char	*user;
	char	*prompt;
}	t_vars;

//BUILT-INS
void	echo(char *msg, int fd, bool option);
int		export_var(char *var, char *value, t_vars *vars);
void	cd(char *path);
void	pwd(t_vars *vars);
void	unset(char *var, t_vars *vars);
void	env(char **envp);
void	print_exit(void);
//EXECUTION
void	here_doc(int fd, char *limiter);
int		execute(t_vars *vars, t_nodes **cmds);
int		exec_routine(t_vars *vars, t_nodes **cmds, int pipes[2][2]);
void	close_pipe(int pipes[2][2], int to_close);
void	exec_cmd(t_vars *vars, t_nodes *cmds, int pipes[2][2]);
char	*get_tmp(void);
//ENVIRONMENT
int		transfer_env(char **envp, t_vars *vars);
void	actualize_env(t_vars *vars);
char	*ft_getenv(char **envp, char *var);
//READLINE PROMPT
int		get_prompt(t_vars *vars);
//FREE AND CLOSE
void	close_child_fds(t_vars *vars, int pipes[2][2]);
void	exit_fd_error(t_vars *vars, int pipes[2][2]);
void	exit_error(char *path, char **envp, char **argv, int status);
void	close_fds(int pipes[2][2], t_vars *vars);
void	free_all(t_vars *vars, char **to_not_free, bool in_child);
void	free_branch(t_nodes *tree, char **to_not_free);
void	free_string_array(char **ptr);

#endif
