/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:04:49 by npapash           #+#    #+#             */
/*   Updated: 2025/03/22 09:04:49 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "expander.h"

char	*handle_single_quote(char *str, int *i, char *result)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		result = handle_regular_char(str, i, result);
	if (str[*i] == '\'')
		(*i)++;
	return (result);
}

char	*handle_double_quote(t_quote_ctx *ctx)
{
	if (ctx->str[*(ctx->i)] == '"')
		ctx->result = handle_double_quote_char(ctx);
	return (ctx->result);
}

static int	process_directory_entries(char **new_argv, int j, char *arg)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return (j);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && is_pattern_match(arg, entry->d_name))
		{
			new_argv[j] = ft_strdup(entry->d_name);
			j++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (j);
}

int	copy_expanded_entries(char **new_argv, int j, char *arg)
{
	int				start_j;

	start_j = j;
	if (!has_unquoted_wildcard(arg))
	{
		new_argv[j] = ft_strdup(arg);
		return (j + 1);
	}
	j = process_directory_entries(new_argv, j, arg);
	if (j == start_j)
		new_argv[j] = ft_strdup(arg);
	return (j + 1);
}
