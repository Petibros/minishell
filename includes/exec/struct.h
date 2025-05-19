/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sacgarci <sacgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:44:40 by sacgarci          #+#    #+#             */
/*   Updated: 2025/05/18 13:33:22 by sacgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <stdbool.h>
# include <limits.h>

typedef struct s_env
{
	int		current_size;
	int		alloc_size;
	char	**envp;
}	t_env;

typedef struct s_line
{
	int			n_cmd;
	int			fd_in;
	int			fd_out;
	char		*line;
	char		**formatted_line;
	bool		append_out;
}	t_line;

typedef struct s_cmds
{
	struct s_nodes	*cmds;
	int				last_exit_status;
	int				last_pid;
	int				pipes[2][2];
	int				pipes_count;
	bool			is_heredoc;
	int				fd_in;
	int				fd_out;
}	t_cmds;

typedef struct s_vars
{
	char	*line;
	t_env	env;
	t_cmds	cmd;
	void	(*sa_setup)(void);
	char	abs_path[PATH_MAX];
	char	*home_path;
	char	*user;
	char	*prompt;
}	t_vars;

#endif
