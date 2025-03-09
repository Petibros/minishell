#include "minishell.h"

int parse_line(char *line, char **envp, t_cmds *cmds)
{
    t_token *tokens;

    if(!cmds)
        return (0);
    tokens = tokenize_line(line);
    (void) tokens;
    (void) envp;
    (void) line;
    return (1);
}