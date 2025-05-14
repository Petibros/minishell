/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:24:00 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/14 11:37:04 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	calc_len(t_vars *vars, char *str, int *n)
{
	int		i;
	char	*var_ptr;
	char	*var_name;

	i = 0;
	while (str[i])
	{
		if (str[i++] == '$')
		{
			var_name = get_expand_name(str, &i);
			if (!var_name)
				*n = -1;
			if (!var_name)
				return ;
			var_ptr = ft_getenv(vars->env.envp, var_name);
			free(var_name);
			if (var_ptr)
				*n += ft_strlen(var_ptr) - 1;
			if (!var_ptr && str[i - 1] == '$' && str[i] == '?')
				*n += count_digits(vars->cmd.last_exit_status) - 1;
			if (!var_ptr && str[i - 1] == '$' && str[i] == '?')
				++i;
		}
		++(*n);
	}
}

static int	compare_after_dollar(t_vars *vars, char *str,
		char *expanded, int *i)
{
	char	*var_name;
	char	*var_ptr;

	var_name = get_expand_name(str, i);
	if (!var_name)
		return (-1);
	else if (!*var_name && !(str[*i - 1] == '$' && str[*i] == '?'))
	{
		free(var_name);
		return (-2);
	}
	var_ptr = ft_getenv(vars->env.envp, var_name);
	free(var_name);
	if (var_ptr)
	{
		ft_strlcpy(expanded, var_ptr, ft_strlen(var_ptr) + 1);
		return (ft_strlen(var_ptr));
	}
	else if (!var_ptr && str[*i - 1] == '$' && str[*i] == '?')
	{
		*i += 1;
		expand_digits(expanded, vars->cmd.last_exit_status);
		return (count_digits(vars->cmd.last_exit_status));
	}
	return (0);
}

static int	go_through_string(t_vars *vars, char *str, char *expanded)
{
	int	i;
	int	j;
	int	n;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			++i;
			n = compare_after_dollar(vars, str, &expanded[j], &i);
			if (n >= 0)
			{
				j += n;
				continue ;
			}
			else if (n == -1)
				return (0);
			else if (n == -2)
				--i;
		}
		expanded[j++] = str[i++];
	}
	return (1);
}

char	*expand_heredoc(t_vars *vars, char *str)
{
	int		n;
	char	*expanded;

	n = 0;
	calc_len(vars, str, &n);
	if (n != -1)
		expanded = ft_calloc(1 + n, sizeof(char));
	if (n == -1 || !expanded)
	{
		perror("In expand heredoc");
		return (NULL);
	}
	if (!go_through_string(vars, str, expanded))
	{
		free(expanded);
		perror("In expand heredoc");
		return (NULL);
	}
	return (expanded);
}
