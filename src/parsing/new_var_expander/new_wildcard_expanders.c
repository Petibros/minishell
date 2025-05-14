/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_wildcard_expanders.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 09:23:03 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/14 09:28:31 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	new_match_pattern(const char *pattern, const char *str)
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

static char	*new_join_matches(char **matches, int count)
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

static int	new_should_expand(const char *str, int pos)
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

static char	*new_process_wildcards(char *str)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				count;
	char			*result;
	char			*pattern;

	dir = opendir(".");
	if (!str || !dir)
		return (ft_strdup(str));
	matches = malloc(sizeof(char *) * 1024);
	if (!matches)
		return (ft_strdup(str));
	count = 0;
	pattern = str;
	if (ft_strncmp(str, "./", 2) == 0)
		pattern = str + 2;
	entry = readdir(dir);
	while (entry && count < 1024)
	{
		if ((pattern[0] == '.' || entry->d_name[0] != '.')
			&& new_match_pattern(pattern, entry->d_name))
		{
			if (ft_strncmp(str, "./", 2) == 0)
			{
				matches[count] = ft_strjoin("./", entry->d_name);
			}
			else
			{
				matches[count] = ft_strdup(entry->d_name);
			}
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	result = new_join_matches(matches, count);
	while (--count >= 0)
		free(matches[count]);
	free(matches);
	if (!result)
		result = ft_strdup(str);
	return (result);
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

static int	new_count_array_size(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}

static char    **new_copy_array(char **src, char **dest, int *index)
{
    int i;

    i = 0;
    while (src && src[i])
    {
        dest[*index] = ft_strdup(src[i]);
        (*index)++;
        i++;
    }
    return (dest);
}

static char    **new_join_string_arrays(char **arr1, char **arr2)
{
    char    **result;
    int     total_size;
    int     j;

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

static char	**new_split_expanded_string(char *expanded)
{
	char	**result;

	if (!expanded)
		return (NULL);
	result = supra_split(expanded, " ");
	if (!result)
		return (NULL);
	return (result);
}

static void	new_free_arr(char **array)
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

char	**new_expand_wildcards_array(char **array)
{
	char	**result;
	char	**temp;
	char	*expanded;
	char	**new_result;
	int		i;

	if (!array)
		return (NULL);
	result = malloc(sizeof(char *) * 1);
	if (!result)
		return (NULL);
	result[0] = NULL;
	i = 0;
	while (array[i])
	{
		expanded = new_expand_wildcard(array[i]);
		if (expanded)
		{
			temp = new_split_expanded_string(expanded);
			if (temp)
			{
				new_result = new_join_string_arrays(result, temp);
				new_free_arr(result);
				new_free_arr(temp);
				result = new_result;
				if (!result)
				{
					free(expanded);
					return (NULL);
				}
			}
			free(expanded);
		}
		i++;
	}
	return (result);
}

static void    new_expand_redirs(t_redir *redirs)
{
	char	*tmp;

	if (!redirs)
		return ;
	while (redirs)
	{
		tmp = redirs->filename;
		redirs->filename = new_expand_wildcard(redirs->filename);
		free(tmp);
		redirs = redirs->next;
	}
}

void	new_expand_wildcards_in_node(t_nodes *node)
{
	char	**argv;

	if (!node)
		return ;
	argv = node->argv;
	if (argv)
	{
		node->argv = new_expand_wildcards_array(argv);
		new_free_arr(argv);
	}
	if (node->file_in)
		new_expand_redirs(node->file_in);
	if (node->file_out)
		new_expand_redirs(node->file_out);
}
