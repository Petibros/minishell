#include "minishell.h"

int	unset_null(char **envp, int i, int *current_size)
{
	perror("export.c, already_exists()");
	--(*current_size);
	while (envp[i + 1])
	{
		envp[i] = envp[i + 1];
		envp[i + 1] = NULL;
		++i;
	}
	return (-1);
}

int	solely_export(char **envp)
{
	int	i;
	int	var_len;

	i = 0;
	while (envp[i])
	{
		var_len = ft_strnlen(envp[i], '=') + 1;
		printf("declare -x %.*s\"%s\"\n", var_len, envp[i], &envp[i][var_len]);
		++i;
	}
	return (0);
}
