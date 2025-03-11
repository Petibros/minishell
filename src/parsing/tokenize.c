#include "minishell.h"

static int handle_quotes(char *line, int *index)
{
    char    quote;
    int     start;

    start = *index;
    quote = line[start];
    (*index)++;
    while (line[*index] != quote && line[*index] != '\0')
        (*index)++;
    if (line[*index] == quote)
        (*index)++;
    else
        *index = start;
    return (*index);
}

static void find_substr(char *line, int *i)
{
    (*i)++;
    while (line[*i] != '\0' && !is_whitespace(line[*i]))
    {
        if ((line[*i] == '"' || line[*i] == '\'') && ft_strchr(&line[*i + 1], line[*i]))
            break ;
        (*i)++;
    }
}

static int process_token(char *line, int *i, t_list **head)
{
    char *token_value;
    int start;

    start = *i;
    if (line[*i] == '"' || line[*i] == '\'')
        *i = handle_quotes(line, i);
    if (*i == start)
        find_substr(line, i);
    token_value = ft_substr(line, start, *i - start);
    if (token_value == NULL)
    {
        ft_lstclear(head, free);
        return (0); 
    }
    t_list *new_node = ft_lstnew(token_value);
    if (!new_node)
    {
        free(token_value);
        ft_lstclear(head, free); 
        return (0); 
    }   
    ft_lstadd_back(head, new_node); 
    return (1); 
}

t_list *tokenize_line(char *line)
{
    t_list *head;
    int     i;

    head = NULL;
    i = 0;
    while (line[i] != '\0')
    {
        i = skip_whitespace(line, &i);
        if (line[i] == '\0')
            break;
        if (!process_token(line, &i, &head))
        {
            ft_lstclear(&head, free);
            return (NULL);
        }
    }
    return (head); 
}