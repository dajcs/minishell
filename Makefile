# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/01 14:10:19 by anemet            #+#    #+#              #
#    Updated: 2025/08/04 16:51:55 by anemet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Source files
SRCS = src/main.c \
       src/parsing/tokenizer.c \
	   src/parsing/tokenizer_utils.c \
       src/parsing/parser.c \
	   src/parsing/parser_utils.c \
       src/parsing/expander.c 
# 	   src/execution/executor.c \
# 	   src/execution/path_finder.c

# Object files
OBJS = $(SRCS:.c=.o)

# --- Libraries ---
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Include path for headers (readline.h: `dpkg -L libreadline-dev`)
# -I/usr/include/readline/ is not needed, /usr/include searched by default
INCLUDES = -I./include -Ilibft

# Linker flags for readline & libft
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -L$(LIBFT_DIR)
# Some systems might need -lncurses, others -ltinfo as well
LDLIBS = -lreadline -lncurses -lft

# ------- Rules ----------

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

# Build the libft whenever needed
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Phony targets
.PHONY: all clean fclean re

