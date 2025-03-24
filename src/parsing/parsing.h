/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:58:07 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/24 15:44:52 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "libft.h"

typedef struct s_nodes
{
    char            *cmd;
    char            **argv;
    int             fd_in;
    int             fd_out;
    int             append_out;
    int             here_doc;
    char            *delimiter;
    int             next_operator;
    struct s_nodes  *left;
    struct s_nodes  *right;
}	t_nodes;

typedef struct s_cmds
{
    t_nodes *cmds;
    int     last_exit_status;
    int     pipes_count;
	int		*fd_in;
	int		*fd_out;
	int		pipes[2][2];
}	t_cmds;

typedef struct s_syntax
{
    int is_command;
    int is_in_redirect;
    int is_out_redirect;
    int is_append_out;
    int is_here_doc;
} t_syntax;

int		parse_line(char *line, char **envp, t_cmds *cmds);
t_list	*tokenize_line(char *line);
int		is_whitespace(char c);
int		skip_whitespace(char *line, int *index);
int     is_syntax_error(t_list *tokens);

#endif
