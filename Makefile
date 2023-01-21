# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 19:58:12 by mmassarw          #+#    #+#              #
#    Updated: 2023/01/21 07:11:31 by hakaddou         ###   ########.fr        #
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
			SRCS/execution/ft_unset.c \
			SRCS/execution/ft_exit.c \
			SRCS/execution/env_conv.c \
			SRCS/execution/ft_cd.c \
			SRCS/execution/get_path_srcs.c \
			SRCS/execution/design.c

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

re:			fclean all clean

exec:		all
			@./$(NAME)

rexec:		re
			@./$(NAME)

leaks:
			make re && make clean \
			&& valgrind --leak-check=full \
			--suppressions=.rl.supp \
			--track-origins=yes \
			--show-leak-kinds=all -s \
			./minishell

.PHONY: all clean fclean re