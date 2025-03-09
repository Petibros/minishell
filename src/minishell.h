/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:51:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/06 00:14:09 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef struct s_token
{
    char			*value;
    struct s_token	*next;
} t_token;

typedef struct s_cmds
{
    char    **cmds;
    char    **argvs;
    char    **envp;
    int     fd_in;
    int     fd_out;
    int     cmd_count;
    char    *input;
    char    *delimiter;
    char    *here_doc;
}   t_cmds;

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
	char	abs_path[PATH_MAX];
	char	*home_path;
	char	*user;
	char	*prompt;
}	t_vars;

void	transfer_env(char **envp, t_vars *vars);
void	actualize_env(t_vars *vars);
void	export_var(char **envp, char *var, char *value, t_vars *vars);
void	cd(char *path);
int		get_prompt(t_vars *vars);
void	pwd(t_vars *vars);
void	unset(char *var, t_vars *vars);
void	env(char **envp);
void	print_exit(void);
int		get_prompt(t_vars *vars);

// Parsing
int		parse_line(char *line, char **envp, t_cmds *cmds);
t_token	*tokenize_line(char *line);
t_token	*create_token_node(char *value, t_token *next);
t_token	*add_token(t_token *head, char *token_value);
void	free_token_list(t_token *head);


#endif
