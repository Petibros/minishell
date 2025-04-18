/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:51:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/17 06:01:31 by sacha            ###   ########.fr       */
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
# include "parsing/types.h"
# include "libft.h"
# include "parsing.h"
# include "signals.h"

typedef struct s_env
{
	int		current_size;
	int		alloc_size;
	char	**envp;
}	t_env;

typedef struct s_line
{
	int			n_cmd;
	int			fd_in;
	int			fd_out;
	char		*line;
	char		**formatted_line;
	bool		append_out;
}	t_line;

typedef struct s_cmds
{
	struct s_nodes	*cmds;
	int				last_exit_status;
	int				last_pid;
	int				pipes[2][2];
	int				pipes_subshell[2][2];
	int				pipes_count;
	int				pipes_count_sub;
	bool			is_heredoc;
	int				fd_in;
	int				fd_out;
}	t_cmds;

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

//PARSING
int		parse_line(t_vars *vars);

//BUILT-INS
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
//EXECUTION
void	heredoc_gestion(t_vars *vars, t_redir *files, int *fd);
int		here_doc(int fd, char *limiter);
void	execute(t_vars *vars, t_nodes *cmds);
int		exec_routine(t_vars *vars, t_nodes *cmds, int is_pipe[2]);
void	close_pipe(int pipes[2][2], int to_close);
void	exec_cmd(t_vars *vars, t_nodes *cmds);
char	*get_tmp(void);
int		wait_processes(int last_known_exit_status, int last_known_pid);
//ENVIRONMENT
int		transfer_env(char **envp, t_vars *vars);
void	actualize_env(t_vars *vars);
char	*ft_getenv(char **envp, char *var);
//READLINE PROMPT
int		get_prompt(t_vars *vars);
//FREE AND CLOSE
void	exit_and_free(t_vars *vars, int status, bool write_exit);
void	close_child_fds(t_vars *vars, int pipes[2][2],
			int pipes_subshell[2][2]);
void	exit_fd_error(t_vars *vars, int pipes[2][2]);
void	exit_no_cmd(t_vars *vars, int pipes[2][2]);
void	exit_error(char *path, char **envp, char **argv, int status);
void	close_fds(t_vars *vars);
void	free_all(t_vars *vars, char **to_not_free, bool in_child);
void	free_branch(t_nodes *tree, char **to_not_free);
void	free_redir(t_redir *node);
void	free_string_array(char **ptr);

#endif
