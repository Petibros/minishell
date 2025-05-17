/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:36:23 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/17 10:22:04 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	verif_argv(char *argv)
{
	int		i;
	size_t	sign;

	sign = 0;
	i = 0;
	if (*argv == '-' || *argv == '+')
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
		if (*argv != '-'
			&& ft_strncmp(argv + sign, "9223372036854775807", 19) > 0)
			return (1);
		else if (*argv == '-'
			&& ft_strncmp(argv + sign, "9223372036854775808", 19) > 0)
			return (1);
	}
	return (0);
}

void	exit_and_free(t_vars *vars, int status, bool write_exit)
{
	free_all(vars, NULL, false);
	if (write_exit)
		write(2, "exit\n", 5);
	exit(status);
}

int	exit_built_in(char **argv, t_vars *vars, bool write_exit, int exit_status)
{
	if (write_exit)
		write(2, "exit\n", 5);
	if (argv[1])
	{
		if (verif_argv(argv[1]) == 0)
		{
			exit_status = (char)ft_atoi(argv[1]);
			if (argv[1] && argv[2])
			{
				write(2, "exit: too many arguments\n", 25);
				vars->cmd.last_exit_status = 1;
				return (1);
			}
		}
		else
		{
			write(2, "exit: ", 5);
			write(2, argv[1], ft_strlen(argv[1]));
			write(2, ": numeric argument required\n", 28);
			exit_status = 2;
		}
	}
	rl_clear_history();
	exit_and_free(vars, exit_status, 0);
	return (0);
}
