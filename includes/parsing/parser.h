/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 19:20:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/23 19:20:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"

// Forward declaration
typedef struct s_vars t_vars;

// Processing contexts
typedef struct s_and_or_ctx
{
	t_nodes		*current;
	t_token		**token;
	t_token_type	op_type;
	char		**envp;
	t_vars		*vars;
}	t_and_or_ctx;

typedef struct s_token_loop_ctx
{
	t_nodes		*node;
	t_token		**token;
	t_token		**word_tokens;
	int			*word_count;
	char		**envp;
}	t_token_loop_ctx;

// Command processing context
typedef struct s_cmd_ctx
{
	t_nodes		*node;
	t_token		**token;
	t_token		**word_tokens;
	int			*word_count;
	char		**envp;
}	t_cmd_ctx;

// Parser functions
t_nodes		*parse(t_token *token, char **envp, t_vars *vars);
t_nodes		*parse_and_or(t_token **token, char **envp, t_vars *vars);
t_nodes		*parse_pipeline(t_token **token, char **envp, t_vars *vars);
t_nodes		*parse_command(t_token **token, char **envp, t_vars *vars);
t_nodes		*create_parser_node(void);
t_nodes		*pratt_parse(t_token *token, char **envp, t_vars *vars);

// Validation functions
int			is_redirection(t_token_type type);
int			is_dollar_operator(const char *value);
int			validate_redirection(t_token *next);
int			validate_parentheses_content(t_token *current);
int			validate_syntax(t_token *tokens);

// Parser command utils
t_token		*create_word_token(t_token *src);
int			process_word_token(t_token **word_tokens, t_token **token, \
								int *word_count);
int			process_command_token(t_cmd_ctx *ctx);
int			build_argv(t_nodes *node, t_token *word_tokens, int word_count);

// Pratt parser functions
t_nodes		*parse_expression(t_token **token, int min_precedence, char **envp, t_vars *vars);
t_nodes		*parse_atom(t_token **token, char **envp, t_vars *vars);
t_nodes		*handle_parentheses(t_token **token, char **envp, t_vars *vars);
int			get_precedence(t_token_type type);
int			count_word_tokens(t_token **token);
void		advance_token(t_token **token, int count);

#endif
