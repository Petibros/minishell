/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "wildcard_expander.h"

int	has_unquoted_wildcard(char *str)
{
	int			in_quotes;
	char		quote;

	in_quotes = 0;
	while (*str)
	{
		if (!in_quotes && *str == '*')
			return (1);
		if (!in_quotes && (*str == '\'' || *str == '"'))
		{
			in_quotes = 1;
			quote = *str;
		}
		else if (in_quotes && *str == quote)
			in_quotes = 0;
		str++;
	}
	return (0);
}

int	is_pattern_match(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
		return (0);
	if (*pattern == '*')
		return (is_pattern_match(pattern + 1, str)
			|| (*str != '\0' && is_pattern_match(pattern, str + 1)));
	if (*pattern == *str)
		return (*str != '\0' && is_pattern_match(pattern + 1, str + 1));
	return (0);
}

int	count_matching_entries(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && is_pattern_match(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

char	**collect_matching_entries(char *pattern, int count)
{
	DIR		*dir;
	char	**matches;

	matches = init_matches_array(count);
	if (!matches)
		return (NULL);
	dir = open_and_validate(matches);
	if (!dir)
		return (NULL);
	fill_matches_array(matches, pattern, count, dir);
	closedir(dir);
	return (matches);
}
