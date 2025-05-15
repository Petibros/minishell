/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:22:20 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:22:20 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
# define SHARED_H

# include "types.h"

/* Shared functions between var and wildcard expanders */
void	new_remove_quotes(char *str);
int		new_count_array_size(char **array);
char	**new_copy_array(char **src, char **dest, int *index);
char	**new_join_string_arrays(char **arr1, char **arr2);
char	**new_split_expanded_string(char *expanded);
void	new_free_arr(char **array);
char	**merge_and_free(char **result, char **temp);

#endif
