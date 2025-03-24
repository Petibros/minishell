/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "types.h"

// Quote handling context
typedef struct s_quote_ctx
{
	char	*str;
	int		*i;
	char	*result;
	int		exit_status;
	char	**envp;
}	t_quote_ctx;

// var_expander.c
char		*expand_variables(char *str, int exit_status, char **envp);

// var_expander_utils1.c
void		expand_variables_in_node(t_nodes *node, int exit_status, \
									char **envp);

// var_expander_utils2.c
char		*expand_env_var(char *str, int *i, int exit_status, char **envp);
t_quote_ctx	*init_quote_context(char *str, int exit_status, char **envp);

// wildcard_expander.c
int			is_pattern_match(const char *pattern, const char *str);

// wildcard_utils.c
void		expand_wildcards(t_nodes *node);

// wildcard_utils2.c
int			count_and_expand_entry(char *arg, char **new_argv, int *j);
void		update_node_cmd(t_nodes *node);

// wildcard_expander_utils.c
DIR			*open_and_validate(char **matches);
char		**init_matches_array(int count);
void		fill_matches_array(char **matches, char *pattern,
				int count, DIR *dir);

// quote_handler.c
char		*handle_double_quote_char(t_quote_ctx *ctx);

// quote_utils.c
char		*handle_single_quote(char *str, int *i, char *result);
char		*handle_double_quote(t_quote_ctx *ctx);

// quote_handler_utils.c
char		*append_substring(char *result, char *str, int start, int end);
char		*handle_regular_char(char *str, int *i, char *result);
char		*handle_quote_char(char *str, int *i, char *result);

#endif
