/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:58:07 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/15 22:18:57 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "libft.h"
# include "minishell.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef struct s_nodes
{
	char			*cmd;
	char			**argv;
	char			*file_in;
	char			*file_out;
	int				append_out;
	int				here_doc;
	char			*delimiter;
	int				is_operator;
	t_token_type	operator_type;
	struct s_nodes	*left;
	struct s_nodes	*right;
}	t_nodes;

# include "lexer.h"
# include "parser.h"
# include "nodes.h"
# include "utils.h"
# include "expander.h"
# include "wildcard_expander.h"

#endif
