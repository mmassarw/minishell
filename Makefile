# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 19:58:12 by mmassarw          #+#    #+#              #
#    Updated: 2023/01/15 22:45:26 by mmassarw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CC		=	@gcc
CFLAGS	=	-g -Wall -Wextra -Werror # -fsanitize=address
RLFLAGS =	-L/usr/local/lib -I/usr/local/include -lreadline

RM		=	@rm -f
ECHO	=	@echo

MAKELIB	=	@make -C libft
LIBFT	=	libft/libft.a
CLNLIB	=	@make clean -C libft
FCLNLIB	=	@make fclean -C libft

SRCS	=	SRCS/main.c \
			SRCS/execution/execution.c \
			SRCS/execution/builtins.c \
			SRCS/execution/ft_echo.c \
			SRCS/execution/ft_export.c \
			SRCS/execution/export_utils.c \
			SRCS/parsing/ft_parse_env.c \
			SRCS/parsing/ft_parse_token.c \
			SRCS/parsing/ft_parse_token_utils.c \
			SRCS/utils/ft_free_list.c \
			SRCS/utils/ft_print_list.c \
			SRCS/utils/temp_funs.c \

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

leaks:
			make re && make clean \
			&& valgrind --leak-check=full \
			--track-origins=yes \
			--show-leak-kinds=all -s \
			./minishell

.PHONY: all clean fclean re