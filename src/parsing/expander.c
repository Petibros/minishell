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

static char	*expand_env_var(char *str, int *i, int exit_status)
{
	char	*var_name;
	char	*var_value;
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
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

static int	is_pattern_match(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *str == '\0')
		return (0);
	if (*pattern == '*')
		return (is_pattern_match(pattern + 1, str) ||
				is_pattern_match(pattern, str + 1));
	if (*pattern == *str)
		return (is_pattern_match(pattern + 1, str + 1));
	return (0);
}

static char	**expand_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				count;
	int				i;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	count = 0;
	while ((entry = readdir(dir)) != NULL)
		if (entry->d_name[0] != '.' && is_pattern_match(pattern, entry->d_name))
			count++;
	closedir(dir);
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	dir = opendir(".");
	i = 0;
	while ((entry = readdir(dir)) != NULL)
		if (entry->d_name[0] != '.' && is_pattern_match(pattern, entry->d_name))
			result[i++] = ft_strdup(entry->d_name);
	result[i] = NULL;
	closedir(dir);
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
		{
			// Copy everything in single quotes literally
			tmp = ft_substr(str, i, 1);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			i++;
			while (str[i] && str[i] != '\'')
			{
				tmp = ft_substr(str, i, 1);
				result = ft_strjoin_free(result, tmp);
				free(tmp);
				i++;
			}
			if (str[i])
			{
				tmp = ft_substr(str, i, 1);
				result = ft_strjoin_free(result, tmp);
				free(tmp);
				i++;
			}
		}
		else if (str[i] == '"')
		{
			// Copy quote
			tmp = ft_substr(str, i, 1);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			i++;
			// Inside double quotes - expand variables but keep other chars
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
				{
					tmp = expand_env_var(str, &i, exit_status);
					if (tmp)
					{
						result = ft_strjoin_free(result, tmp);
						free(tmp);
					}
				}
				else
				{
					tmp = ft_substr(str, i, 1);
					result = ft_strjoin_free(result, tmp);
					free(tmp);
					i++;
				}
			}
			if (str[i])
			{
				tmp = ft_substr(str, i, 1);
				result = ft_strjoin_free(result, tmp);
				free(tmp);
				i++;
			}
		}
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
		{
			tmp = ft_substr(str, i, 1);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			i++;
		}
	}
	return (result);
}

void	expand_variables_in_node(t_nodes *node, int exit_status)
{
	char	*expanded;
	int		i;

	if (!node || !node->argv)
		return ;

	for (i = 0; node->argv[i]; i++)
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
	}

	if (node->left)
		expand_variables_in_node(node->left, exit_status);
	if (node->right)
		expand_variables_in_node(node->right, exit_status);
}

void	expand_wildcards(t_nodes *node)
{
	char	**expanded;
	char	**new_argv;
	int		i;
	int		j;
	int		count;

	if (!node || !node->argv)
		return ;
	count = 0;
	for (i = 0; node->argv[i]; i++)
	{
		if (ft_strchr(node->argv[i], '*'))
		{
			expanded = expand_wildcard(node->argv[i]);
			if (expanded)
			{
				for (j = 0; expanded[j]; j++)
					count++;
				free_array(expanded);
			}
		}
		else
			count++;
	}
	new_argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return ;
	j = 0;
	for (i = 0; node->argv[i]; i++)
	{
		if (ft_strchr(node->argv[i], '*'))
		{
			expanded = expand_wildcard(node->argv[i]);
			if (expanded)
			{
				for (int k = 0; expanded[k]; k++)
					new_argv[j++] = expanded[k];
				free(expanded);
			}
		}
		else
			new_argv[j++] = ft_strdup(node->argv[i]);
	}
	new_argv[j] = NULL;
	free_array(node->argv);
	node->argv = new_argv;

	if (node->left)
		expand_wildcards(node->left);
	if (node->right)
		expand_wildcards(node->right);
}
