/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_wildcard_expanders2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:38:38 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:38 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_directory(DIR *dir, char *pattern, char ***matches)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if ((pattern[0] == '.' || entry->d_name[0] != '.')
			&& new_match_pattern(pattern, entry->d_name))
		{
			if (add_match(matches, &count, pattern, entry->d_name) == -1)
				return (-1);
		}
		entry = readdir(dir);
	}
	return (count);
}

void	free_matches(char **matches, int count)
{
	while (count >= 1)
	{
		free(matches[count - 1]);
		--count;
	}
	free(matches);
}

char	*new_process_wildcards(char *str)
{
	DIR		*dir;
	char	**matches;
	int		count;
	char	*result;
	char	*pattern;

	dir = opendir(".");
	if (!str || !dir)
		return (ft_strdup(str));
	matches = malloc(sizeof(char *));
	if (!matches)
	{
		closedir(dir);
		return (handle_no_matches(str, matches));
	}
	count = 0;
	pattern = str;
	if (ft_strncmp(str, "./", 2) == 0)
		pattern = str + 2;
	count = process_directory(dir, pattern, &matches);
	closedir(dir);
	if (count == -1)
		return (handle_no_matches(str, matches));
	result = new_join_matches(matches, count);
	free_matches(matches, count);
	if (result)
		return (result);
	return (ft_strdup(str));
}

char	*new_expand_wildcard(char *str)
{
	int		i;
	int		has_wildcard;

	if (!str)
		return (NULL);
	i = 0;
	has_wildcard = 0;
	while (str[i])
	{
		if (str[i] == '*' && new_should_expand(str, i))
		{
			has_wildcard = 1;
			break ;
		}
		i++;
	}
	if (has_wildcard)
		return (new_process_wildcards(str));
	return (ft_strdup(str));
}

char	**new_expand_wildcards_array(char **array)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * 1);
	if (!array || !result)
		return (NULL);
	result[0] = NULL;
	i = 0;
	while (array[i])
	{
		result = handle_expansion(result, array[i]);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}
