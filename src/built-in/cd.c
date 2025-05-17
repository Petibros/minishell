/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:19:03 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/17 11:15:00 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	join_and_export(char *var_name, char *path,
		char **tmp_export, t_vars *vars)
{
	tmp_export[0] = "export";
	tmp_export[1] = ft_strjoin(var_name, path);
	tmp_export[2] = NULL;
	if (!tmp_export[1])
	{
		free(tmp_export);
		return (0);
	}
	if (export_var(tmp_export, &vars->env.envp, vars) != 0)
		return (0);
	free(tmp_export[1]);
	return (1);
}

static int	change_pwd(t_vars *vars, char old_pwd[PATH_MAX], int update_env)
{
	char	pwd[PATH_MAX];
	char	**tmp_export;

	if (!update_env)
		write(2, "cd: cannot update OLDPWD var\n", 29);
	if (!getcwd(pwd, PATH_MAX) && !update_env)
	{
		write(2, "cd: cannot update PWD var: ", 27);
		perror("");
		return (0);
	}
	tmp_export = malloc(3 * sizeof(char *));
	if (!tmp_export)
		return (-1);
	if (update_env && ft_getenv(vars->env.envp, "OLDPWD")
		&& !join_and_export("OLDPWD=", old_pwd, tmp_export, vars))
		return (-1);
	if (!join_and_export("PWD=", pwd, tmp_export, vars))
		return (-1);
	free(tmp_export);
	return (0);
}

int	cd(char **argv, t_vars *vars)
{
	char	old_pwd[PATH_MAX];
	int		update_env;

	update_env = 1;
	if (!getcwd(old_pwd, PATH_MAX))
		update_env = 0;
	if (!argv[1] && chdir(ft_getenv(vars->env.envp, "HOME")) == -1)
	{
		write(2, "cd: HOME not set\n", 17);
		return (1);
	}
	else if (argv[1] && argv[2])
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	else if (argv[1] && chdir(argv[1]) == -1)
	{
		write(2, "cd: ", 4);
		perror(argv[1]);
		return (1);
	}
	return (change_pwd(vars, old_pwd, update_env));
}
