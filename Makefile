# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 00:07:40 by sacgarci          #+#    #+#              #
#    Updated: 2025/04/02 20:53:28 by sacha            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc

CFILES =			src/main.c \
		 			src/prompt.c \
		 			src/getenv.c \
		 			src/here_doc.c

CFILES_BUILT-IN =	src/built-in/pwd.c \
					src/built-in/unset.c \
					src/built-in/env.c \
					src/built-in/exit.c \
					src/built-in/cd.c \
					src/built-in/export.c \
					src/built-in/export_utils.c \
					src/built-in/echo.c

CFILES_EXEC =		src/exec/exec.c \
					src/exec/pipe.c \
					src/exec/child.c \
					src/exec/exit_child.c

CFILES_PARSING =	src/parsing/lexer/lexer.c \
					src/parsing/lexer/lexer_operator.c \
					src/parsing/lexer/lexer_word.c \
					src/parsing/parser/parser.c \
					src/parsing/parser/parser_node.c \
					src/parsing/parser/parser_command.c \
					src/parsing/parser/parser_command_utils.c \
					src/parsing/parser/parser_pipeline.c \
					src/parsing/parser/parser_and_or.c \
					src/parsing/parser/parser_parentheses.c \
					src/parsing/parser/pratt_parser.c \
					src/parsing/parser/pratt_parser_atom.c \
					src/parsing/parser/parse_line.c \
					src/parsing/expanders/quote_handler.c \
					src/parsing/expanders/var_expander.c \
					src/parsing/expanders/var_expander_utils1.c \
					src/parsing/expanders/var_expander_utils2.c \
					src/parsing/expanders/wildcard_expander.c \
					src/parsing/expanders/wildcard_expander_utils.c \
					src/parsing/expanders/token_var_expander.c \
					src/parsing/expanders/token_wildcard_expander.c \
					src/parsing/nodes/pratt_node.c \
					src/parsing/nodes/pratt_node_op.c \
					src/parsing/nodes/pratt_node_cmd.c \
					src/parsing/utils/pratt_utils.c \
					src/parsing/utils/quote_utils.c \
					src/parsing/utils/quote_utils2.c \
					src/parsing/utils/redirection_utils1.c \
					src/parsing/utils/redirection_utils2.c \
					src/parsing/utils/redirection_utils3.c \
					src/parsing/utils/redirection_utils4.c \
					src/parsing/utils/parsing_utils.c \
					src/parsing/utils/expansion_quote_utils.c \
					src/parsing/utils/wildcard_utils.c \
					src/parsing/utils/wildcard_utils2.c \
					src/parsing/utils/quote_handler_utils.c \
					src/parsing/utils/print_ast.c \
					src/parsing/utils/array_utils.c

CFILES_FREE =		src/free/free.c

CFILES_SIGNALS =	src/signals/signals.c \
					src/signals/handle_sigint.c \
					src/signals/handle_sigquit.c

OFILES = $(CFILES:.c=.o) $(CFILES_PARSING:.c=.o) $(CFILES_BUILT-IN:.c=.o) $(CFILES_FREE:.c=.o) $(CFILES_EXEC:.c=.o) $(CFILES_SIGNALS:.c=.o)
LIBFT = libft/libft.a
HEADER = src/minishell.h 
HEADER_PARSING = includes/parsing/parsing.h  
HEADER_LIBFT = libft/libft.h 
CFLAGS = -Wall -Wextra -Werror -g -I libft -I src -I includes -I includes/parsing -I includes/signals
LDFLAGS = $(LIBFT) -lreadline

all : $(NAME)

$(NAME) : $(LIBFT) $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) -o $(NAME)

$(LIBFT) :
	make bonus -C libft

%.o : %.c $(HEADER_PARSING) $(HEADER_LIBFT) $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

run : all
	./$(NAME)

clean :
	make clean -C libft
	rm -f $(OFILES)

fclean : clean
	rm -f $(LIBFT)
	rm -f $(NAME)

re : fclean all

.PHONY = all run clean fclean re
