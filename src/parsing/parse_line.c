#include "minishell.h"
/*
typedef struct s_cmds
{
    t_list  *cmds;
    t_list  *argvs;
    char    **envp;
    int     fd_in;
    int     fd_out;
    int     cmd_count;
} t_cmds;
*/

void print_tokens(t_list *tokens)
{
    t_list *current;

    current = tokens;
    while (current != NULL)
    {
        char *token_value = (char *)current->content;
        printf("%s\n", token_value);
        current = current->next;
    }
}

int parse_line(char *line, char **envp, t_cmds *cmds)
{
    t_list  *tokens;

    (void)cmds;
    (void)envp;
    tokens = tokenize_line(line);
    if (!tokens)
        return (0);
    print_tokens(tokens);
    if (is_syntax_error(tokens))
    {
        ft_lstclear(&tokens, free);
        printf("Syntax error!\n");
        return (0);
    }
    ft_lstclear(&tokens, free);
    return (1);
}