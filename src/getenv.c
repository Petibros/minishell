#include "minishell.h"

char	*ft_getenv(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, ft_strlen(var)) == 0 && envp[i][ft_strlen(var)] == '=')
			return (envp[i] + ft_strlen(var) + 1);
		++i;
	}
	return (NULL);
}

int	transfer_env(char **envp, t_vars *vars)
{
	int	i;

	i = 0;
	while (envp[i])
		++i;
	vars->env.current_size = i;
	vars->env.alloc_size = i;
	vars->env.envp = malloc((vars->env.alloc_size + 1) * sizeof(char *));
	if (!vars->env.envp)
		return (-1);
	i = 0;
	while (envp[i])
	{
		vars->env.envp[i] = ft_strdup(envp[i]);
		if (!vars->env.envp[i])
			return (-1);
		++i;
	}
	vars->env.envp[i] = NULL;
	return (1);
}

void	actualize_env(t_vars *vars)
{
	vars->user = ft_getenv(vars->env.envp, "USER");
	vars->home_path = ft_getenv(vars->env.envp, "HOME");
	getcwd(vars->abs_path, PATH_MAX);
}
