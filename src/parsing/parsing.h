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
	int				fd_in;
	int				fd_out;
	int				append_out;
	int				here_doc;
	char			*delimiter;
	int				next_operator;
	struct s_nodes	*left;
	struct s_nodes	*right;
}	t_nodes;

typedef struct s_cmds
{
	t_nodes	*cmds;
	int		last_exit_status;
	int		pipes_count;
	int		fd_in;
	int		fd_out;
}	t_cmds;

/* Lexer functions */
t_token		*lexer(char *input);
t_token		*get_next_token(char **input);

/* Parser functions */
t_nodes		*parse(t_token *token);

/* Expander functions */
char		*expand_variables(char *str, int exit_status);
void		expand_variables_in_node(t_nodes *node, int exit_status);
void		expand_wildcards(t_nodes *node);

/* Quote handling functions */
char		*remove_quotes(char *str);
int			check_quotes(char *str);
void		handle_quotes_in_node(t_nodes *node);

/* Redirection functions */
int			handle_redirections(t_nodes *node, t_token **token);

/* Utility functions */
void		free_array(char **array);
void		free_token(t_token *token);
void		free_node(t_nodes *node);
char		*ft_strjoin_free(char *s1, char *s2);
void		print_syntax_error(char *token);

/* Debug functions */
void		print_token_list(t_token *token);
void		print_ast(t_nodes *root);

#endif
