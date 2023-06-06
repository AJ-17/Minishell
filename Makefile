# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajakubcz <ajakubcz@42Lyon.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/01 14:11:17 by ajakubcz          #+#    #+#              #
#    Updated: 2023/06/06 15:43:26 by ajakubcz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
LIBFT = libft/libft.a
DIR_SRCS = srcs/
SRCS = main.c get_prompt.c utils.c
HEADER = minishell.h

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra

all : libft_rule $(NAME)

%.o: $(DIR_SRCS)%.c $(HEADER) $(LIBFT) Makefile
	$(CC) $(CFLAGS) -I ./ -I libft -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

libft_rule :
	make -C libft

clean : 
		rm -rf $(OBJS)
		make clean -C libft

fclean : clean
		rm -rf $(NAME) $(LIBFT)

re : fclean all

.PHONY : all clean fclean re libft_rule
