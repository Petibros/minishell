#include "minishell.h"

int parse_line(char *line, char **envp, t_cmds *cmds)
{
    if(!cmds)
        return (0);
    (void) envp;
    (void) line;
    return (NULL);
}