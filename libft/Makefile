# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sacgarci <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/08 18:03:03 by sacgarci          #+#    #+#              #
#    Updated: 2025/04/02 20:52:49 by sacha            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFILES = ft_atoi_base.c get_next_line_bonus.c get_next_line_utils_bonus.c ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_memcmp.c ft_strnstr.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

CFLAGS = -Wall -Wextra -Werror
OFILES = $(CFILES:.c=.o)

CFILESBONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

OFILESBONUS = $(CFILESBONUS:.c=.o)

all : $(NAME)

$(NAME): $(OFILES)
	ar -rcs $(NAME) $(OFILES)

clean: 
	rm -f $(OFILES) $(OFILESBONUS)

fclean: clean
	rm -f $(NAME)	

re:	fclean all

bonus: $(OFILES) $(OFILESBONUS)
	ar -rcs $(NAME) $(OFILES) $(OFILESBONUS)
