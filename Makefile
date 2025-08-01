# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/01 14:10:19 by anemet            #+#    #+#              #
#    Updated: 2025/08/01 14:21:40 by anemet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Include path for headers (readline.h: `dpkg -L libreadline-dev`)
CPPFLAGS = -I./include -I/usr/include/readline/

# Source files
SRCS = src/main.c \
       src/parsing/tokenizer.c

# Object files
OBSJ = $(SRCS:.c=.o)

# Linker flags for readline
LDFLAGS = -L/usr/lib/x86_64-linux-gnu/libreadline.a
LDLIBS = -lreadline

# ------- Rules ----------

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBSJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Phony targets
.PHONY: all clean fclean re

