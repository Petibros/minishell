/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 10:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 10:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "wildcard_expander.h"

static int	handle_non_wildcard(char *arg, char **new_argv, int *j)
{
	new_argv[*j] = ft_strdup(arg);
	(*j)++;
	return (1);
}

int	process_directory_entries(char *arg, char **new_argv, int *j)
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
		if (entry->d_name[0] != '.' && is_pattern_match(arg, entry->d_name))
		{
			new_argv[*j] = ft_strdup(entry->d_name);
			(*j)++;
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

int	expand_wildcard_entry(char *arg, char **new_argv, int *j)
{
	int	count;

	if (!has_unquoted_wildcard(arg))
		return (handle_non_wildcard(arg, new_argv, j));
	count = process_directory_entries(arg, new_argv, j);
	if (count == 0)
	{
		new_argv[*j] = ft_strdup(arg);
		(*j)++;
		count = 1;
	}
	return (count);
}
