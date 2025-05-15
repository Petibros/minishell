/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_wildcard_expanders1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:38:38 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_match_pattern(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
		return (0);
	if (*pattern == '*')
		return (new_match_pattern(pattern + 1, str)
			|| new_match_pattern(pattern, str + 1));
	if (*pattern == *str)
		return (new_match_pattern(pattern + 1, str + 1));
	return (0);
}

char	*new_join_matches(char **matches, int count)
{
	char	*result;
	char	*temp;
	int		i;

	if (!count)
		return (NULL);
	result = ft_strdup(matches[0]);
	i = 1;
	while (i < count)
	{
		temp = result;
		result = ft_strjoin(result, " ");
		free(temp);
		temp = result;
		result = ft_strjoin(result, matches[i]);
		free(temp);
		i++;
	}
	return (result);
}

int	new_should_expand(const char *str, int pos)
{
	int		in_single;
	int		in_double;
	int		i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (i <= pos)
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '\"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (!in_single);
}

char	*handle_no_matches(char *str, char **matches)
{
	new_free_arr(matches);
	return (ft_strdup(str));
}

int	add_match(char ***matches, int *count, char *str, char *d_name)
{
	char	**new_matches;

	new_matches = realloc(*matches, sizeof(char *) * (*count + 1));
	if (!new_matches)
		return (-1);
	*matches = new_matches;
	if (ft_strncmp(str, "./", 2) == 0)
		(*matches)[*count] = ft_strjoin("./", d_name);
	else
		(*matches)[*count] = ft_strdup(d_name);
	if (!(*matches)[*count])
		return (-1);
	(*count)++;
	return (0);
}
