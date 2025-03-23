/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expander.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_EXPANDER_H
# define WILDCARD_EXPANDER_H

# include "parsing.h"

/* Functions from wildcard_expander.c */
int		is_pattern_match(const char *pattern, const char *str);
int		count_matching_entries(char *pattern);
char	**collect_matching_entries(char *pattern, int count);
int		has_unquoted_wildcard(char *str);
int		copy_expanded_entries(char **new_argv, int j, char *arg);

/* Functions from wildcard_utils2.c */
int		count_and_expand_entry(char *arg, char **new_argv, int *j);
void	update_node_cmd(t_nodes *node);

#endif
