/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var_expander.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:50:12 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_VAR_EXPANDER_H
# define NEW_VAR_EXPANDER_H

# include "types.h"
# include "parsing/shared.h"

/* Variable expansion functions */
void	new_expand_variables_in_node(t_nodes *node, t_vars *vars);
void	new_expand_argv(char **argv, char **envp, t_vars *vars);
char	*new_get_expanded_str(char *str, char **envp, t_vars *vars);
char	*new_handle_expansions(char **str, char **envp, t_vars *vars,
			int *is_in_double_quote);
char	*new_find_next_expansion(char *str, int *is_in_double_quote);
char	**new_expand_argv_with_split(char **argv, char **envp, t_vars *vars);
char	**new_expand_single_argv(char *arg, char **envp, t_vars *vars,
		char **result);
char	*new_add_pre_str(char *current, char *pre_str);
char	*new_get_var_name(char *dollar);
char	*new_get_var_value(char *dollar, char **envp, t_vars *vars);
char	*new_skip_var(char *dollar);
void	join_hole(char **argv);
void	new_expand_redirs(t_redir *redirs, t_vars *vars);

/* Wildcard expansion functions */
void	new_expand_wildcards_in_node(t_nodes *node, t_vars *vars, int *status);
char	**new_expand_wildcards_array(char **array);
char	*new_expand_wildcard(char *str);
char	**handle_expansion(char **result, char *str);
int	new_match_pattern(const char *pattern, const char *str);
char	*new_join_matches(char **matches, int count);
int	new_should_expand(const char *str, int pos);
char	*handle_no_matches(char *str, char **matches);
int	add_match(char ***matches, int *count, char *str, char *d_name);
int	process_directory(DIR *dir, char *pattern, char ***matches);
void	free_matches(char **matches, int count);
char	*new_process_wildcards(char *str);
void	new_expand_redirs_wildcards(t_redir *redirs, t_vars *vars, int *status);

/* Quote handling functions */
void	new_remove_quotes(char *str);
void	remove_all_quotes(t_nodes *node);

/* Utility functions */
char	**merge_and_free(char **result, char **temp);
char	**new_join_string_arrays(char **arr1, char **arr2);
char	**new_split_expanded_string(char *expanded);
void	new_free_arr(char **array);
int	new_count_array_size(char **array);
char	**new_copy_array(char **src, char **dest, int *index);

/* String splitting function */
char	**supra_split(char const *s);

#endif
