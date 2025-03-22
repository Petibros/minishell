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

# include "parsing.h"

// var_expander.c
char	*expand_variables(char *str, int exit_status);
void	expand_variables_in_node(t_nodes *node, int exit_status);

// wildcard_expander.c
int		is_pattern_match(const char *pattern, const char *str);

// wildcard_utils.c
void	expand_wildcards(t_nodes *node);

// wildcard_utils2.c
int		count_and_expand_entry(char *arg, char **new_argv, int *j);
void	update_node_cmd(t_nodes *node);

// quote_handler.c
char	*handle_double_quote_char(char *str, int *i, char *result,
			int exit_status);

// quote_utils.c
char	*handle_single_quote(char *str, int *i, char *result);
char	*handle_double_quote(char *str, int *i, char *result,
			int exit_status);

// quote_handler_utils.c
char	*append_substring(char *result, char *str, int start, int end);
char	*handle_regular_char(char *str, int *i, char *result);
char	*handle_quote_char(char *str, int *i, char *result);
char	*expand_env_var(char *str, int *i, int exit_status);

#endif
