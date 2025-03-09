#include "minishell.h"

t_token *create_token_node(char *value, t_token *next)
{
    t_token *new_node;
    
    new_node = malloc(sizeof(t_token));
    if (!new_node)
        return (NULL);
    new_node->value = value; 
    new_node->next = next;
    return (new_node);
}

t_token *add_token(t_token *head, char *token_value)
{
    t_token *new_token;
    t_token *tail;

    new_token = create_token_node(token_value, NULL);
    if (!new_token)
    {
        free_token_list(head);
        return (NULL); 
    }
    if (!head)
        return (new_token);
    tail = head;
    while (tail->next)
        tail = tail->next;
    tail->next = new_token; 
    return (head);
}

void free_token_list(t_token *head)
{
    t_token *current;
    t_token *next;

    current = head;
    while (current != NULL)
    {
        next = current->next;
        free(current->value); 
        free(current); 
        current = next;
    }
}