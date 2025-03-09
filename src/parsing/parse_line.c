#include "minishell.h"

/*
typedef struct s_line
{
	int			n_cmd;
	int			fd_in;
	int			fd_out;
	char		*line;
	char		**formatted_line;
	bool		append_out;
}	t_line;
*/

t_line	*parse_line(char *line)
{
	t_cmds	cmds;

	cmds = init_cmds();
	return (0);
}