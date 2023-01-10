# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 19:58:12 by mmassarw          #+#    #+#              #
#    Updated: 2023/01/10 01:51:03 by mmassarw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

CC		=	@gcc
CFLAGS	=	-Wall -Wextra -Werror

RM		=	@rm -f

ECHO	=	@echo

SRCS	=	$(wildcards *.c)

OBJS	=	$(patsubst %.c,%.o,$(wildcard *.c))

$(NAME):	$(OBJS)
			$(ECHO) "Compiling libft .........."
			@ar -rcs $(NAME) $(OBJS)

all:		$(NAME)

clean:
			$(ECHO) "Removing obj files ......."
			$(RM) $(OBJS)

fclean:		clean
			$(ECHO) "Removing libft.a files ..."
			$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re