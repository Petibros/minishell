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
CFILES = src/getenv.c src/main.c	src/prompt.c src/pwd.c src/unset.c src/env.c src/exit.c src/cd.c src/export.c
OFILES = $(CFILES:.c=.o)
LIBFT = libft/libft.a
HEADER = src/minishell.h
CFLAGS = -Wall -Wextra -Werror -I libft
LDFLAGS = $(LIBFT) -lreadline

all : $(NAME)

$(NAME) : $(LIBFT) $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) -o $(NAME)

$(LIBFT) :
	make -C libft

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

