# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 00:07:40 by sacgarci          #+#    #+#              #
#    Updated: 2025/03/15 23:31:56 by sacgarci         ###   ########.fr        #
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
					src/built-in/echo.c

#CFILES_EXEC =		src/exec/exec.c \
					src/exec/pipe.c \
					src/exec/child.c

CFILES_PARSING =	src/parsing/lexer.c \
					src/parsing/parser.c \
					src/parsing/pratt_parser.c \
					src/parsing/pratt_node.c \
					src/parsing/pratt_utils.c \
					src/parsing/quotes.c \
					src/parsing/redirections.c \
					src/parsing/utils.c

CFILES_EXPANDER =	src/parsing/expanders/var_expander.c \
					src/parsing/expanders/wildcard_expander.c \
					src/parsing/expanders/wildcard_utils.c \
					src/parsing/expanders/wildcard_utils2.c \
					src/parsing/expanders/quote_handler.c \
					src/parsing/expanders/quote_utils.c \
					src/parsing/expanders/quote_handler_utils.c

OFILES = $(CFILES:.c=.o) $(CFILES_PARSING:.c=.o) $(CFILES_BUILT-IN:.c=.o) $(CFILES_EXPANDER:.c=.o)
LIBFT = libft/libft.a
HEADER = src/minishell.h 
HEADER_PARSING = src/parsing/parsing.h 
HEADER_LIBFT = libft/libft.h 
CFLAGS = -Wall -Wextra -Werror -g -I libft -I src -I src/parsing
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

