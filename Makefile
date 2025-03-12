# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 00:07:40 by sacgarci          #+#    #+#              #
#    Updated: 2025/03/06 00:17:43 by sacgarci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFILES =	src/main.c \
			src/prompt.c \
			src/getenv.c \
			src/pwd.c \
			src/unset.c \
			src/env.c \
			src/exit.c \
			src/cd.c \
			src/export.c \
			src/echo.c \
			src/here_doc.c
CFILES_PARSING =	src/parsing/parse_line.c \
					src/parsing/tokenize.c \
					src/parsing/utils.c

OFILES = $(CFILES:.c=.o) $(CFILES_PARSING:.c=.o)
LIBFT = libft/libft.a
HEADER = src/minishell.h
CFLAGS = -Wall -Wextra -Werror -g -I libft -I src
LDFLAGS = $(LIBFT) -lreadline

all : $(NAME)

$(NAME) : $(LIBFT) $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) -g -o $(NAME)

$(LIBFT) :
	make -C libft bonus

%.o : %.c $(HEADER)
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

