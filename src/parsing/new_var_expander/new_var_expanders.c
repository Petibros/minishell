/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var_expanders.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npapashv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:12:27 by npapashv          #+#    #+#             */
/*   Updated: 2025/05/02 01:58:29 by npapashv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    new_remove_quotes(char *str)
{
    char    quote;
    char    *matching_quote;

    while (*str)
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
        ft_memmove(matching_quote, matching_quote + 1, ft_strlen(matching_quote + 1) + 1);
        str = matching_quote;
    }
}

static char	*new_add_pre_str(char *current, char *pre_str)
{
	char	*result;

	result = ft_strjoin(current, pre_str);
	free(current);
	free(pre_str);
	return (result);
}

static char	*new_get_var_name(char *dollar)
{
	char	*name;
	int		i;

	i = 1;
	while (dollar[i] && (ft_isalpha(dollar[i]) || dollar[i] == '_'))
		++i;
	while (dollar[i] && (ft_isalnum(dollar[i]) || dollar[i] == '_'))
		++i;
	name = ft_substr(dollar, 1, i - 1);
	return (name);
}

static char	*new_get_var_value(char *dollar, char **envp, t_vars *vars)
{
	char	*name;
    char    *value;
    
    if (dollar[1] == '\0')
    {
        return (ft_strdup("$"));
    }
    if (dollar[1] == '?')
		return (ft_itoa(vars->cmd.last_exit_status));
	name = new_get_var_name(dollar);
    value = ft_getenv(envp, name);
    free(name);
    if (!value)
        return (ft_strdup(""));
    return (ft_strdup(value));
}

static char    *new_skip_var(char *dollar)
{
    int		i;

	i = 1;
    if (dollar[1] == '?')
    {
        return (dollar + 2);
    }
	while (dollar[i] && (ft_isalpha(dollar[i]) || dollar[i] == '_'))
		++i;
	while (dollar[i] && (ft_isalnum(dollar[i]) || dollar[i] == '_'))
		++i;
    return (dollar + i);
}

static char	*new_find_next_expansion(char *str, int *is_in_double_quote)
{
    while (str && *str)
    {
        if (*str == '"')
            *is_in_double_quote = !*is_in_double_quote;
        if (*str == '$' && (ft_isalpha(str[1]) || str[1] == '_' || str[1] == '?'))
            return (str) ;
        if (*str == '\'' && !*is_in_double_quote)
            str = ft_strchr(str + 1, '\'');
        if (str)
            ++str;
    }
    return (NULL);
}

char	*new_get_expanded_str(char *str, char **envp, t_vars *vars)
{
	char	*current;
	char	*dollar;
	char	*var_value;
	char	*pre_str;
    char    *tmp;
    int     is_in_double_quote;

	if (!str)
		return (NULL);
    is_in_double_quote = 0;
	current = NULL;
	dollar = new_find_next_expansion(str, &is_in_double_quote);
	while (*str && dollar)
	{
		pre_str = ft_substr(str, 0, dollar - str);
		current = new_add_pre_str(current, pre_str);
		var_value = new_get_var_value(dollar, envp, vars);
        tmp = current;
        current = ft_strjoin(current, var_value);
        free(tmp);
        free(var_value);
        str = new_skip_var(dollar);
		dollar = new_find_next_expansion(str, &is_in_double_quote);
	}
    if (!str)
        return (current);
    pre_str = ft_strdup(str);
    current = new_add_pre_str(current, pre_str);
	return (current);
}

static int	new_count_array_size(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
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

static char	**new_join_string_arrays(char **arr1, char **arr2)
{
	char	**result;
	int		size1;
	int		size2;
	int		i;
	int		j;

	size1 = new_count_array_size(arr1);
	size2 = new_count_array_size(arr2);
	result = malloc(sizeof(char *) * (size1 + size2 + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (arr1 && arr1[i])
	{
		result[j++] = ft_strdup(arr1[i]);
		i++;
	}
	i = 0;
	while (arr2 && arr2[i])
	{
		result[j++] = ft_strdup(arr2[i]);
		i++;
	}
	result[j] = NULL;
	return (result);
}

static  void    join_hole(char **argv)
{
    int     i;
    
    free(argv[0]);
    argv[0] = NULL;
    i = 0;
    while   (argv[i + 1])
    {
        argv[i] = argv[i + 1];
        ++i;
    }
    argv[i] = NULL;
}

char	**new_expand_argv_with_split(char **argv, char **envp, t_vars *vars)
{
    int     i;
    char    *expanded;
    char    **split_result;
    char    **result;
    char    **new_result;

    if (!argv)
        return (NULL);
    
    result = malloc(sizeof(char *) * 1);
    if (!result)
        return (NULL);
    result[0] = NULL;
    
    i = 0;
    while (argv[i])
    {
        expanded = new_get_expanded_str(argv[i], envp, vars);
        if (expanded)
        {
            if (expanded[0] == '\0')
            {
                free(expanded);
                i++;
                continue;
            }
            
            split_result = new_split_expanded_string(expanded);
            if (split_result)
            {
                new_result = new_join_string_arrays(result, split_result);
                new_free_arr(result);
                new_free_arr(split_result);
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

void    new_expand_argv(char **argv, char **envp, t_vars *vars)
{
    int     i;
    char    *tmp;

    if (!argv)
        return ;
    i = 0;
    while (argv[i])
    {
        tmp = argv[i];
        argv[i] = new_get_expanded_str(argv[i], envp, vars);
        free(tmp);
        if (!argv[i] || argv[i][0] == '\0')
        {
            join_hole(argv + i);
            continue ;
        }
        ++i;
    }
}

static void    new_expand_redirs(t_redir *redirs, char **envp, t_vars *vars)
{
    char    *tmp;
    
    if (!redirs)
        return ;
    while (redirs)
    {
        tmp = redirs->filename;
        redirs->filename = new_get_expanded_str(redirs->filename, envp, vars);
        free(tmp);
        redirs = redirs->next;
    }
}

void	new_expand_variables_in_node(t_nodes *node, char **envp, t_vars *vars)
{
    char    **argv;
    char    **new_argv;

	if (!node)
		return ;
    argv = node->argv;
    if (argv)
    {
        new_argv = new_expand_argv_with_split(argv, envp, vars);
        new_free_arr(argv);
        node->argv = new_argv;
    }
    if (node->file_in)
        new_expand_redirs(node->file_in, envp, vars);
    if (node->file_out)
        new_expand_redirs(node->file_out, envp, vars);
}
