/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:19:03 by sacgarci          #+#    #+#             */
/*   Updated: 2025/03/28 00:00:16 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	join_and_export(char *var_name, char *path,
		char **tmp_export, t_vars *vars)
{
	tmp_export[0] = ft_strjoin(var_name, path);
	tmp_export[1] = NULL;
	if (!tmp_export[0])
	{
		free(tmp_export);
		return (0);
	}
	if (!export_var(tmp_export, &vars->env.envp, vars))
		return (0);
	free(tmp_export[0]);
	return (1);
}

static int	change_pwd(t_vars *vars, char old_pwd[PATH_MAX])
{
	char	pwd[PATH_MAX];
	char	**tmp_export;

	getcwd(pwd, PATH_MAX);
	tmp_export = malloc(2 * sizeof(char *));
	if (!tmp_export)
		return (0);
	if (!join_and_export("OLDPWD", old_pwd, tmp_export, vars))
		return (0);
	if (!join_and_export("PWD", pwd, tmp_export, vars))
		return (0);
	free(tmp_export);
	return (1);
}

int	cd(char **argv, t_vars *vars)
{
	char	old_pwd[PATH_MAX];

	getcwd(old_pwd, PATH_MAX);
	if (!argv[1] && chdir(vars->home_path) == -1)
	{
		perror("cd");
		return (1);
	}
	else if (argv[2])
	{
		write(2, "cd: too many arguments", 22);
		return (1);
	}
	else if (chdir(argv[1]) == -1)
	{
		perror("cd");
		return (1);
	}
	if (!change_pwd(vars, old_pwd))
		return (-1);
	return (0);
}
