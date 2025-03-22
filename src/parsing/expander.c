/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapash <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 20:09:08 by npapash           #+#    #+#             */
/*   Updated: 2025/03/18 20:09:08 by npapash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <dirent.h>

static char	*handle_quote_char(char *str, int *i, char *result);
static char	*handle_regular_char(char *str, int *i, char *result);

static char	*expand_env_var(char *str, int *i, int exit_status)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		len;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	len = 0;
	while (ft_isalnum(str[*i + len]) || str[*i + len] == '_')
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(str, *i, len);
	*i += len;
	var_value = getenv(var_name);
	if (!var_value)
		result = ft_strdup("");
	else
		result = ft_strdup(var_value);
	free(var_name);
	return (result);
}

static int	is_pattern_match(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
		return (0);
	if (*pattern == '*')
		return (is_pattern_match(pattern + 1, str) || \
				is_pattern_match(pattern, str + 1));
	if (*pattern == *str)
		return (is_pattern_match(pattern + 1, str + 1));
	return (0);
}

static int	count_matching_entries(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (-1);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && 
			is_pattern_match(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static char	**collect_matching_entries(char *pattern, int count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				i;

	result = malloc(sizeof(char *) * (count + 1));
	dir = opendir(".");
	if (!dir || !result)
	{
		free(result);
		return (NULL);
	}
	i = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && is_pattern_match(pattern, entry->d_name))
			result[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	result[i] = NULL;
	closedir(dir);
	return (result);
}

static char	**expand_wildcard(char *pattern)
{
	int		count;
	char	**matches;

	count = count_matching_entries(pattern);
	if (count < 0)
		return (NULL);
	matches = collect_matching_entries(pattern, count);
	return (matches);
}



static char	*handle_quote_char(char *str, int *i, char *result)
{
	char	*tmp;

	tmp = ft_substr(str, *i, 1);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	(*i)++;
	return (result);
}

static char	*handle_regular_char(char *str, int *i, char *result)
{
	char	*tmp;

	tmp = ft_substr(str, *i, 1);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	(*i)++;
	return (result);
}

static char	*handle_double_quote_char(char *str, int *i, char *result, int exit_status)
{
	char	*tmp;

	if (str[*i] == '$')
	{
		tmp = expand_env_var(str, i, exit_status);
		if (tmp)
		{
			result = ft_strjoin_free(result, tmp);
			free(tmp);
		}
	}
	else
	{
		tmp = ft_substr(str, *i, 1);
		result = ft_strjoin_free(result, tmp);
		free(tmp);
		(*i)++;
	}
	return (result);
}

static char	*handle_single_quote(char *str, int *i, char *result)
{
	result = handle_quote_char(str, i, result);
	while (str[*i] && str[*i] != '\'')
		result = handle_regular_char(str, i, result);
	if (str[*i])
		result = handle_quote_char(str, i, result);
	return (result);
}

static char	*handle_double_quote(char *str, int *i, char *result, int exit_status)
{
	result = handle_quote_char(str, i, result);
	while (str[*i] && str[*i] != '"')
		result = handle_double_quote_char(str, i, result, exit_status);
	if (str[*i])
		result = handle_quote_char(str, i, result);
	return (result);
}

char	*expand_variables(char *str, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			result = handle_single_quote(str, &i, result);
		else if (str[i] == '"')
			result = handle_double_quote(str, &i, result, exit_status);
		else if (str[i] == '$')
		{
			tmp = expand_env_var(str, &i, exit_status);
			if (tmp)
			{
				result = ft_strjoin_free(result, tmp);
				free(tmp);
			}
		}
		else
			result = handle_regular_char(str, &i, result);
	}
	return (result);
}

void	expand_variables_in_node(t_nodes *node, int exit_status)
{
	char	*expanded;
	int		i;

	if (!node)
		return ;

	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			if (ft_strchr(node->argv[i], '$'))
			{
				expanded = expand_variables(node->argv[i], exit_status);
				if (expanded)
				{
					free(node->argv[i]);
					node->argv[i] = expanded;
				}
			}
			i++;
		}
	}

	if (node->left)
		expand_variables_in_node(node->left, exit_status);
	if (node->right)
		expand_variables_in_node(node->right, exit_status);
}

static int	count_expanded_entry(char *arg)
{
	char	**expanded;
	int		j;
	int		count;

	count = 0;
	if (ft_strchr(arg, '*'))
	{
		expanded = expand_wildcard(arg);
		if (expanded)
		{
			j = 0;
			while (expanded[j])
			{
				count++;
				j++;
			}
			free_array(expanded);
		}
	}
	else
		count++;
	return (count);
}

static int	count_expanded_args(char **argv)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (argv[i])
	{
		count += count_expanded_entry(argv[i]);
		i++;
	}
	return (count);
}

static int	has_unquoted_wildcard(const char *str)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '*' && !in_single_quote && !in_double_quote)
			return (1);
		i++;
	}
	return (0);
}

static int	copy_expanded_entries(char **new_argv, int j, char *arg)
{
	char	**expanded;
	int		k;

	if (!has_unquoted_wildcard(arg))
	{
		new_argv[j++] = ft_strdup(arg);
		return (j);
	}
	expanded = expand_wildcard(arg);
	if (expanded)
	{
		k = 0;
		while (expanded[k])
		{
			new_argv[j++] = expanded[k];
			k++;
		}
		free(expanded);
	}
	return (j);
}

static char	**create_expanded_argv(char **argv, int count)
{
	char	**new_argv;
	int		i;
	int		j;

	new_argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return (NULL);
	j = 0;
	i = 0;
	while (argv[i])
	{
		j = copy_expanded_entries(new_argv, j, argv[i]);
		i++;
	}
	new_argv[j] = NULL;
	return (new_argv);
}

static void	expand_node_wildcards(t_nodes *node)
{
	char	**new_argv;
	int		count;

	if (!node || !node->argv)
		return ;
	count = count_expanded_args(node->argv);
	new_argv = create_expanded_argv(node->argv, count);
	if (!new_argv)
		return ;
	free_array(node->argv);
	node->argv = new_argv;
}

void	expand_wildcards(t_nodes *node)
{
	if (!node)
		return ;
	expand_node_wildcards(node);
	if (node->left)
		expand_wildcards(node->left);
	if (node->right)
		expand_wildcards(node->right);
}
