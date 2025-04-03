/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:36:23 by sacgarci          #+#    #+#             */
/*   Updated: 2025/04/03 05:17:28 by sacha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	verif_argv(char *argv)
{
	int		i;
	size_t	sign;

	sign = 0;
	i = 0;
	if (*argv == '-')
		sign = 1;
	while (argv[i + sign])
	{
		if (ft_isdigit(argv[i + sign]) == 0)
			return (1);
		++i;
	}
	if (ft_strlen(argv) > 19 + sign)
		return (1);
	else if (ft_strlen(argv) == 19 + sign)
	{
		if (!sign && ft_strncmp(argv, "9223372036854775807", 19) > 0)
			return (1);
		else if (sign && ft_strncmp(argv + sign, "9223372036854775808", 19) > 0)
			return (1);
	}
	return (0);
}

void	exit_and_free(t_vars *vars, int status)
{
	int	term;

	term = open("/dev/tty", O_WRONLY);
	if (term == -1)
		perror("cannot write exit in term");
	free_all(vars, NULL, false);
	if (term != -1)
		write(term, "exit\n", 5);
	close(term);
	exit(status);
}

int	exit_built_in(char **argv, t_vars *vars)
{
	int	exit_status;

	exit_status = vars->cmd.last_exit_status;
	if (argv[1] && argv[2])
	{
		write(2, "exit: too many arguments\n", 25);
		vars->cmd.last_exit_status = 2;
		return (2);
	}
	if (argv[1])
	{
		if (verif_argv(argv[1]) == 0)
			exit_status = (char)ft_atoi(argv[1]);
		else
		{
			write(2, "exit: ", 5);
			write(2, argv[1], ft_strlen(argv[1]));
			write(2, ": numeric argument required\n", 28);
			exit_status = 2;
		}
	}
	exit_and_free(vars, exit_status);
	return (0);
}
