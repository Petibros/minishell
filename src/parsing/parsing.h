/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:58:07 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/14 17:07:17 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "libft.h"

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
