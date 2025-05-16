/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:22:20 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:22:20 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**new_copy_array(char **src, char **dest, int *index)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		dest[*index] = ft_strdup(src[i]);
		(*index)++;
		i++;
	}
	return (dest);
}

char	**new_join_string_arrays(char **arr1, char **arr2)
{
	char	**result;
	int		total_size;
	int		j;

	total_size = new_count_array_size(arr1) + new_count_array_size(arr2);
	result = malloc(sizeof(char *) * (total_size + 1));
	if (!result)
		return (NULL);
	j = 0;
	result = new_copy_array(arr1, result, &j);
	result = new_copy_array(arr2, result, &j);
	result[j] = NULL;
	return (result);
}

int	new_count_array_size(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}

char	**new_split_expanded_string(char *expanded)
{
	char	**result;
	char	*temp;

	if (!expanded)
		return (NULL);
	temp = ft_strdup(expanded);
	if (!temp)
		return (NULL);
	result = supra_split(temp);
	free(temp);
	if (!result)
		return (NULL);
	return (result);
}

char	**merge_and_free(char **result, char **temp)
{
	char	**new_result;

	new_result = new_join_string_arrays(result, temp);
	new_free_arr(result);
	new_free_arr(temp);
	return (new_result);
}
