/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:51:15 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/05 17:20:06 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

#	include <linux/limits.h>
#	include <unistd.h>
#	include <stdio.h>
#	include <fcntl.h>
#	include <signal.h>
#	include <stdlib.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <stdbool.h>
#	include <readline/history.h>
#	include <readline/readline.h>
#	include "../libft/libft.h"

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
	char	abs_path[PATH_MAX];
	char	*home_path;
	char	*user;
	char	*prompt;
}	t_vars;

int		get_prompt(t_vars *vars);

// Parsing
int		parse_line(char *line, char **envp, t_cmds *cmds);
t_token	*tokenize_line(char *line);
t_token	*create_token_node(char *value, t_token *next);
t_token	*add_token(t_token *head, char *token_value);
void	free_token_list(t_token *head);


#endif