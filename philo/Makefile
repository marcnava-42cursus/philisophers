# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcnava <marcnava@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 18:03:32 by marcnava          #+#    #+#              #
#    Updated: 2025/05/20 16:31:50 by marcnava         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# ********************************* VARIABLES ******************************** #

NAME				:=	philo

CC					:=	cc
CFLAGS				:=	-Wall -Wextra -pthread
DFLAGS				:=	-g3

COMPILER			:=	$(CC) $(CFLAGS) $(DFLAGS)

RM					:=	rm -rf

# **************************************************************************** #
# *********************************** FILES ********************************** #

SRCPATH				:=	src
BUILDPATH			:=	build

UTILSPATH			:=	$(SRCPATH)/utils

INCLUDES			:=	-Iincludes

SRCS				:=	$(SRCPATH)/init.c \
						$(SRCPATH)/philo.c

SRCS				+=	$(UTILSPATH)/debug.c \
						$(UTILSPATH)/errors.c \
						$(UTILSPATH)/libft.c

OBJS				:=	$(SRCS:$(SRCPATH)/%.c=$(BUILDPATH)/%.o)

# **************************************************************************** #
# *********************************** RULES ********************************** #

all:				$(NAME)

$(NAME):			$(OBJS)
	$(CC) $(INCLUDES) $(OBJS) -o $(NAME)

$(BUILDPATH)/%.o:	$(SRCPATH)/%.c
	mkdir -p $(@D)
	$(COMPILER) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(BUILDPATH)

fclean:				clean
	$(RM) $(NAME)

re:					fclean all

# **************************************************************************** #
# *********************************** PHONY ********************************** #

.PHONY: all
.PHONY: clean
.PHONY: fclean
.PHONY: re
