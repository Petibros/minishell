#include "minishell.h"

static char	*ft_getenv(char **envp, char *val)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], val, ft_strlen(val) + 1) == 0)
			return (envp[i] + ft_strlen(val) + 1);
		++i;
	}
	return (NULL);
}

void	transfer_env(char **envp, t_vars *vars)
{
	int	i;

	i = 0;
	while (envp[i])
		++i;
	vars->env.current_size = i;
	vars->env.alloc_size = i;
	vars->env.envp = malloc(vars->env.alloc_size * sizeof(char *) + 1);
	i = 0;
	while (envp[i])
	{
		vars->env.envp[i] = ft_strdup(envp[i]);
		++i;
	}
	vars->env.envp[i] = NULL;
}

int	actualize_env(t_vars *vars)
{
	vars->user = ft_getenv("USER");
	vars->home_path = ft_getenv("HOME");
	getcwd(vars->abs_path, PATH_MAX);
	return (0);
}
