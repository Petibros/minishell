/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:51:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/14 16:27:14 by sacgarci         ###   ########.fr       */
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
# include <sys/stat.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../libft/libft.h"

typedef struct s_cmds
{
    t_list  *cmds;
    t_list  *argvs;
	char	**delimiter;
    char	*in_files;
    char	*out_files;
	int		last_code;
	int		*next_operator;
	int		*append_fd_out;
	int		*here_doc;
	int		pipes_count;
    int     cmd_count;
} t_cmds;

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

typedef struct s_vars
{
	t_line	input;
	t_env	env;
	t_cmds	cmd;
	char	abs_path[PATH_MAX];
	char	*home_path;
	char	*user;
	char	*prompt;
}	t_vars;

void	here_doc(int fd, char *limiter);
void	echo(char *msg, int fd, bool option);
int		transfer_env(char **envp, t_vars *vars);
void	actualize_env(t_vars *vars);
int		export_var(char *var, char *value, t_vars *vars);
void	cd(char *path);
int		get_prompt(t_vars *vars);
void	pwd(t_vars *vars);
void	unset(char *var, t_vars *vars);
void	env(char **envp);
void	print_exit(void);
int		get_prompt(t_vars *vars);
char	*ft_getenv(char **envp, char *var);

// Parsing
int		parse_line(char *line, char **envp, t_cmds *cmds);
t_list	*tokenize_line(char *line);
int		is_whitespace(char c);
int		skip_whitespace(char *line, int *index);

#endif
