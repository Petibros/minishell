#include "minishell.h"

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int skip_whitespace(char *line, int *index)
{
    while (line[*index] != '\0' && is_whitespace(line[*index]))
        (*index)++;
    return (*index);
}