#include "minishell.h"

static void remove_redir_quotes(t_redir *redir)
{
    while (redir)
	{
		new_remove_quotes(redir->filename);
		redir = redir->next;
	}
}

void	remove_all_quotes(t_nodes *node)
{
	int		i;

	if (!node)
		return ;
	i = 0;
	while (node->argv && node->argv[i])
	{
		new_remove_quotes(node->argv[i]);
		++i;
	}
	remove_redir_quotes(node->file_in);
	remove_redir_quotes(node->file_out);
	remove_redir_quotes(node->heredoc);
}