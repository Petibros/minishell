/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expander_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:49:04 by npapash           #+#    #+#             */
/*   Updated: 2025/03/24 17:49:04 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

DIR	*open_and_validate(char **matches)
{
	DIR	*dir;

	dir = opendir(".");
	if (!dir)
	{
		free(matches);
		return (NULL);
	}
	return (dir);
}

char	**init_matches_array(int count)
{
	char	**matches;

	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	return (matches);
}

void	fill_matches_array(char **matches, char *pattern,
			int count, DIR *dir)
{
	struct dirent	*entry;
	int				i;

	i = 0;
	entry = readdir(dir);
	while (entry && i < count)
	{
		if (entry->d_name[0] != '.' && is_pattern_match(pattern, entry->d_name))
		{
			matches[i] = ft_strdup(entry->d_name);
			i++;
		}
		entry = readdir(dir);
	}
	matches[i] = NULL;
}
