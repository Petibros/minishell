#include "minishell.h"

static int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int skip_whitespace(char *line, int *index)
{
    while (line[*index] != '\0' && is_whitespace(line[*index]))
        (*index)++;
    return (*index);
}

static int handle_quotes(char *line, int *index)
{
    char    quote;
    int     start;

    quote = line[*index];
    start = *index;
    (*index)++;
    while (line[*index] != quote && line[*index] != '\0')
        (*index)++;
    if (line[*index] == quote)
        (*index)++;
    else
        *index = start;
    return (*index);
}

static int process_token(char *line, int *i, t_token **head)
{
    char    *token_value;
    int     start;

    start = *i;
    if (line[*i] == '"' || line[*i] == '\'')
        *i = handle_quotes(line, i);
    while (line[*i] != '\0' && !is_whitespace(line[*i]))
        (*i)++;
    token_value = ft_substr(line, start, *i - start);
    if (token_value == NULL)
    {
        free_token_list(*head);
        return (0);
    }
    *head = add_token(*head, token_value);
    if (*head == NULL)
        return (0);
    return (1);
}

t_token *tokenize_line(char *line)
{
    t_token *head;
    int i;

    head = NULL;
    i = 0;
    while (line[i] != '\0')
    {
        i = skip_whitespace(line, &i);
        if (line[i] == '\0')
            break;
        if (!process_token(line, &i, &head))
            return (NULL);
    }
    return (head);
}