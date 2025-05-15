/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var_expander.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:50:12 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/14 23:53:18 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEW_VAR_EXPANDER_H
# define NEW_VAR_EXPANDER_H

# include "types.h"
# include "parsing/shared.h"

void	new_expand_variables_in_node(t_nodes *node, t_vars *vars);
void	new_expand_argv(char **argv, char **envp, t_vars *vars);
char	*new_get_expanded_str(char *str, char **envp, t_vars *vars);

void	new_expand_wildcards_in_node(t_nodes *node, t_vars *vars, int *status);
char	**new_expand_wildcards_array(char **array);
char	*new_expand_wildcard(char *str);

void	new_remove_quotes(char *str);
void	remove_all_quotes(t_nodes *node);

char	**handle_expansion(char **result, char *str);
char	**merge_and_free(char **result, char **temp);

char	**supra_split(char const *s, char *seps);

#endif
