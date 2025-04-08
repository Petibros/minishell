#include "parsing.h"

static void	print_indent(int depth)
{
	while (depth-- > 0)
		printf("  ");
}

static const char	*get_token_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_AND)
		return ("AND");
	else if (type == TOKEN_OR)
		return ("OR");
	else if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	else if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == TOKEN_APPEND)
		return ("APPEND");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_LPAREN)
		return ("LPAREN");
	else if (type == TOKEN_RPAREN)
		return ("RPAREN");
	else if (type == TOKEN_SUBSHELL)
		return ("SUBSHELL");
	else if (type == TOKEN_EOF)
		return ("EOF");
	return ("UNKNOWN");
}

void	print_ast_types(t_nodes *node, int depth)
{
	if (!node)
		return ;

	print_indent(depth);
	if (node->is_operator)
		printf("%s\n", get_token_type_str(node->operator_type));
	else
		printf("CMD\n");

	print_ast_types(node->left, depth + 1);
	print_ast_types(node->right, depth + 1);
}
