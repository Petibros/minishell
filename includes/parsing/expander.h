/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/04/11 22:32:41 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "types.h"

typedef struct s_quote_ctx
{
	char	*str;
	int		i;
	char	*result;
	int		exit_status;
	char	**envp;
}	t_quote_ctx;

typedef struct s_token_processor_ctx
{
	t_token	*prev;
	t_token	*next;
	int		exit_status;
	char	**envp;
}	t_token_processor_ctx;

char		*expand_variables(char *str, int exit_status, char **envp);
char		*post_process_expansion(char *result, char *str);
void		expand_variables_in_node(t_nodes *node, int exit_status,
				char **envp);
void		expand_variables_in_tokens(t_token **tokens, int exit_status,
				char **envp);
void		expand_token_value(t_token *token, int exit_status, char **envp);
char		*expand_env_var(char *str, int *i, int exit_status, char **envp);
t_quote_ctx	*init_quote_context(char *str, int exit_status, char **envp);
char		*get_var_value(char *var_name, int exit_status, char **envp);
char		*get_var_name(char *str, int *i);
char		*handle_special_var_cases(char *str, int *i, int exit_status);
char		*handle_underscore_case(char *var_name, int exit_status,
				char **envp);
char		*handle_dollar_sign(t_quote_ctx *ctx);
char		*handle_in_squote(t_quote_ctx *ctx);
char		*extract_variable_part(char *result, char *var_in_result,
				int var_len);
int			get_var_length(char *var_name);
int			find_next_dollar(char *str, int *in_squote);
char		*process_variable(char *result, char *start);
void		process_char(t_quote_ctx *ctx);
int			is_pattern_match(const char *pattern, const char *str);
void		expand_wildcards_in_tokens(t_token *tokens);
void		expand_wildcards(t_nodes *node);
int			count_and_expand_entry(char *arg, char **new_argv, int *j);
void		update_node_cmd(t_nodes *node);
DIR			*open_and_validate(char **matches);
char		**init_matches_array(int count);
void		fill_matches_array(char **matches, char *pattern,
				int count, DIR *dir);
char		*handle_double_quote_char(t_quote_ctx *ctx);
char		*handle_single_quote(char *str, int *i, char *result);
char		*append_substring(char *result, char *str, int start, int end);
char		*handle_regular_char(char *str, int *i, char *result);
char		*handle_quote_char(char *str, int *i, char *result);
void		handle_empty_token(t_token **tokens, t_token *current,
				t_token *prev, t_token *next);
t_token		*process_token(t_token **tokens, t_token *current,
				t_token_processor_ctx *ctx);
char		*ft_strjoin_array(char **array, char *delimiter);

#endif
