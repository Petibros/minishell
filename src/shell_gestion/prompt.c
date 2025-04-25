/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:17:58 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/05 19:55:55 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_prompt_full_path(t_vars *vars)
{
	char	*tmp;

	vars->prompt = ft_strjoin(vars->user, " : ");
	if (!vars->prompt)
		return (-1);
	tmp = ft_strjoin(vars->prompt, vars->abs_path);
	if (!tmp)
	{
		free(vars->prompt);
		return (-1);
	}
	free(vars->prompt);
	vars->prompt = ft_strjoin(tmp, " > ");
	if (!vars->prompt)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	return (0);
}

static int	get_prompt_from_home(t_vars *vars)
{
	char	*tmp;

	vars->prompt = ft_strjoin(vars->user, " : ~");
	if (!vars->prompt)
		return (-1);
	tmp = ft_strjoin(vars->prompt,
			vars->abs_path + ft_strlen(vars->home_path));
	if (!tmp)
	{
		free(vars->prompt);
		return (-1);
	}
	free(vars->prompt);
	vars->prompt = ft_strjoin(tmp, " > ");
	if (!vars->prompt)
	{
		free(tmp);
		return (-1);
	}
	free(tmp);
	return (0);
}

int	get_prompt(t_vars *vars)
{
	if (!vars->user)
		vars->user = "";
	if (vars->home_path && ft_strnstr(vars->abs_path, vars->home_path,
			ft_strlen(vars->home_path)) == vars->abs_path)
	{
		if (get_prompt_from_home(vars) == -1)
			return (-1);
	}
	else
	{
		if (get_prompt_full_path(vars) == -1)
			return (-1);
	}
	return (0);
}
