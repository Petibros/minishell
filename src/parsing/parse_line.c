#include "minishell.h"

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
    t_list *tokens;

    (void)envp;
    tokens = tokenize_line(line);
    if (!tokens)
        return (0);
    // print_tokens(tokens);
    ft_lstclear(&tokens, free);
    (void)cmds;
    return (1);
}