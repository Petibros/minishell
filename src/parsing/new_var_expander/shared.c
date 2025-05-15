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

void	new_remove_quotes(char *str)
{
	char	quote;
	char	*matching_quote;

	while (str && *str)
	{
		if (*str != '"' && *str != '\'')
		{
			++str;
			continue ;
		}
		quote = *str;
		ft_memmove(str, str + 1, ft_strlen(str + 1) + 1);
		matching_quote = ft_strchr(str, quote);
		if (!matching_quote)
			break ;
		ft_memmove(matching_quote, matching_quote + 1,
			ft_strlen(matching_quote + 1) + 1);
		str = matching_quote;
	}
}

int	new_count_array_size(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}

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

char	**new_split_expanded_string(char *expanded)
{
	char	**result;

	if (!expanded)
		return (NULL);
	result = supra_split(expanded);
	if (!result)
		return (NULL);
	return (result);
}

void	new_free_arr(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**merge_and_free(char **result, char **temp)
{
	char	**new_result;

	new_result = new_join_string_arrays(result, temp);
	new_free_arr(result);
	new_free_arr(temp);
	return (new_result);
}
