/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expanders.c                                  :+:      :+:    :+:   */
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

static char	*new_get_expanded_str(char *str, char **envp, t_vars *vars)
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
        new_remove_quotes(argv[i]);
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
        new_remove_quotes(redirs->filename);
        free(tmp);
        redirs = redirs->next;
    }
}

void	new_expand_variables_in_node(t_nodes *node, char **envp, t_vars *vars)
{
    char    **argv;

	if (!node)
		return ;
    argv = node->argv;
    if (argv)
    {
        new_expand_argv(argv, envp, vars);
        node->argv = argv;
    }
    if (node->file_in)
        new_expand_redirs(node->file_in, envp, vars);
    if (node->file_out)
        new_expand_redirs(node->file_out, envp, vars);
}