# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 19:58:12 by mmassarw          #+#    #+#              #
#    Updated: 2023/01/11 16:41:21 by hakaddou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CC		=	@gcc
CFLAGS	=	-g -Wall -Wextra -Werror
RLFLAGS =	-L/usr/local/lib -I/usr/local/include -lreadline

RM		=	@rm -f
ECHO	=	@echo

MAKELIB	=	@make -C libft
LIBFT	=	libft/libft.a
CLNLIB	=	@make clean -C libft
FCLNLIB	=	@make fclean -C libft

SRCS	=	SRCS/main.c

OBJS	=	$(SRCS:.c=.o)

$(NAME):	$(OBJS)
			$(MAKELIB)
			$(ECHO) "Compiling minishell ......"
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(RLFLAGS) 

all:		$(NAME)

clean:
			$(CLNLIB)
			$(RM) $(OBJS)

fclean:		
			$(FCLNLIB)
			$(ECHO) "Removing minishell ......."
			$(RM) $(NAME) $(OBJS)

re:			fclean all

exec:		all
			@./$(NAME)

rexec:		re
			@./$(NAME)

.PHONY: all clean fclean re