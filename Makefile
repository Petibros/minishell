# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 00:07:40 by sacgarci          #+#    #+#              #
#    Updated: 2025/05/14 09:32:51 by npapashv         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc

CFILES_SHELL =		src/shell_gestion/main.c \
		 			src/shell_gestion/prompt.c \
		 			src/shell_gestion/getenv.c \
		 			src/shell_gestion/getenv_utils.c \
					src/shell_gestion/minishell.c

CFILES_BUILT-IN =	src/built-in/pwd.c \
					src/built-in/unset.c \
					src/built-in/env.c \
					src/built-in/exit.c \
					src/built-in/cd.c \
					src/built-in/export.c \
					src/built-in/export_utils.c \
					src/built-in/echo.c

CFILES_EXEC =		src/exec/exec.c \
					src/exec/setup_exec.c \
					src/exec/child.c \
					src/exec/exit_child.c \
					src/exec/wait.c \
					src/exec/file_descriptors.c \
					src/exec/get_path.c \
					src/exec/here_doc.c \
					src/exec/expand_heredoc.c \
					src/exec/expand_heredoc_utils.c \
					src/exec/exec_utils.c

CFILES_PARSING =	src/parsing/lexer/lexer.c \
					src/parsing/lexer/lexer_operator.c \
					src/parsing/lexer/lexer_word.c \
					src/parsing/lexer/lexer_word2.c \
					src/parsing/parser/parser_command.c \
					src/parsing/parser/parser_command_utils.c \
					src/parsing/parser/parser_pipeline.c \
					src/parsing/parser/parser_and_or.c \
					src/parsing/parser/pratt_parser.c \
					src/parsing/parser/pratt_parser_atom.c \
					src/parsing/parser/parse_line.c \
					src/parsing/parser/parse_line_validation.c \
					src/parsing/parser/parse_line_validation_utils.c \
					src/parsing/nodes/pratt_node_op.c \
					src/parsing/utils/pratt_utils.c \
					src/parsing/utils/pratt_utils2.c \
					src/parsing/utils/pratt_utils3.c \
					src/parsing/utils/redirection_utils1.c \
					src/parsing/utils/redirection_utils2.c \
					src/parsing/utils/redirection_utils3.c \
					src/parsing/utils/redirection_utils4.c \
					src/parsing/utils/parsing_utils.c \
					src/parsing/utils/quote_utils.c \
					src/parsing/new_var_expander/new_var_expanders1.c \
					src/parsing/new_var_expander/new_var_expanders2.c \
					src/parsing/new_var_expander/new_var_expanders3.c \
					src/parsing/new_var_expander/new_wildcard_expanders1.c \
					src/parsing/new_var_expander/new_wildcard_expanders2.c \
					src/parsing/new_var_expander/new_wildcard_expanders3.c \
					src/parsing/new_var_expander/supra_split.c \
					src/parsing/new_var_expander/remove_all_quotes.c \
					src/parsing/new_var_expander/shared.c

CFILES_FREE =		src/free/free.c \
					src/free/close.c

CFILES_SIGNALS =	src/signals/signals.c \
					src/signals/handle_sigint.c

OFILES = $(CFILES_SHELL:.c=.o) $(CFILES_PARSING:.c=.o) $(CFILES_BUILT-IN:.c=.o) $(CFILES_FREE:.c=.o) $(CFILES_EXEC:.c=.o) $(CFILES_SIGNALS:.c=.o)
LIBFT = libft/libft.a
HEADER = src/minishell.h 
HEADER_PARSING = includes/parsing/parsing.h  
HEADER_LIBFT = libft/libft.h 
CFLAGS = -Wall -Wextra -Werror -g -I libft -I src -I includes -I includes/parsing -I includes/signals -I includes/exec
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
