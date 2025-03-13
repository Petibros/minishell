#include "minishell.h"

void    init_syntax(t_syntax *syntax, int value)
{
    syntax->is_command = value;
    syntax->is_in_redirect = value;
    syntax->is_out_redirect = value;
    syntax->is_append_out = value;
    syntax->is_here_doc = value;
}

int is_operator(char *token_content)
{
    if (ft_strncmp(token_content, "|", 1) == 0)
        return (1);
    if (ft_strncmp(token_content, "<", 1) == 0)
        return (1);
    if (ft_strncmp(token_content, ">", 1) == 0)
        return (1);
    if (ft_strncmp(token_content, "<<", 2) == 0)
        return (1);
    if (ft_strncmp(token_content, ">>", 2) == 0)
        return (1);
    return (0);
}

int manage_operator(t_syntax *syntax, t_list *token)
{
    if (token->next == NULL)
        return (0);
    if (is_operator((char *)token->next->content))
        return (0);
    if (ft_strncmp((char *)token->content, "<", 1) == 0)
        syntax->is_in_redirect += 1;
    if (ft_strncmp((char *)token->content, ">", 1) == 0)
        syntax->is_out_redirect += 1;
    if (ft_strncmp((char *)token->content, "<<", 2) == 0)
        syntax->is_here_doc += 1;
    if (ft_strncmp((char *)token->content, ">>", 2) == 0)
        syntax->is_append_out += 1;
    return (1);
}

int is_command_valid(t_list **token)
{
    t_list      *current_token;
    t_syntax    syntax;

    current_token= *token;
    init_syntax(&syntax, 0);
    if (ft_strncmp((char *)current_token->content, "|", 1) == 0)
        current_token = current_token->next;
    while (current_token && ft_strncmp((char *)current_token->content, "|", 1) != 0)
    {
        if (is_operator((char *)current_token->content))
        {
            if (!manage_operator(&syntax, current_token))
                return (0);
            current_token = current_token->next;
        }
        else
            syntax.is_command += 1;
        current_token = current_token->next;
    }
    if (syntax.is_command == 0)
        return (0);
    if (syntax.is_in_redirect > 1 || syntax.is_out_redirect > 1)
        return (0);
    if (syntax.is_append_out > 1 || syntax.is_here_doc > 1)
        return (0);
    *token = current_token;
    return (1);
}

int is_syntax_error(t_list *tokens)
{
    t_list      *current_token;

    current_token = tokens;
    while (current_token)
    {
        if (!is_command_valid(&current_token))
            return (1);
    }
    return (0);
}