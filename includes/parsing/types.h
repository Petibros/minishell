/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 22:22:17 by npapash           #+#    #+#             */
/*   Updated: 2025/04/04 22:53:42 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>

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
	TOKEN_SUBSHELL,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef struct s_redir
{
	char			*filename;
	bool			append;
	struct s_redir	*next;
}	t_redir;

typedef struct s_nodes
{
	char			**argv;
	t_redir			*file_in;
	t_redir			*file_out;
	t_redir			*heredoc;
	bool			is_operator;
	t_token_type	operator_type;
	struct s_nodes	*left;
	struct s_nodes	*right;
}	t_nodes;

#endif
