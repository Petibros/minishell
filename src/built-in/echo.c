#include "minishell.h"

static int	is_option(char *str)
{
	int	i;

	i = 1;
	if (*str != '-')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		++i;
	}
	return (1);
}

static int	get_beginning(char **argv, bool *option)
{
	int	i;

	i = 1;
	while (argv[i] && is_option(argv[i]))
	{
		*option = true;
		++i;
	}
	return (i);
}

void	echo(char **argv, char **envp)
{
	bool	option;
	int		beginning;
	int		status;

	option = false;
	status = 0;
	beginning = get_beginning(argv, &option);
	while (argv[beginning])
	{
		if (write(1, argv[beginning], ft_strlen(argv[beginning])) == -1)
		{
			status = 125;
			perror("echo");
		}
		++beginning;
		if (argv[beginning])
			write(1, " ", 1);
	}
	if (!option)
		write(1, "\n", 1);
	free_string_array(argv);
	free_string_array(envp);
	exit(status);
}
