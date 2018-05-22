
# makefile for mkrandom

CC = gcc

CFLAGS = -O3 -Wall -Wextra -Wuninitialized -Wunused -Werror -Wformat=2 -Wno-unused-parameter -Wwrite-strings -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wformat-security -std=gnu99 -s -march=native -mtune=native -flto

NAME = mkrandom

$(NAME): $(NAME).o
	$(CC) $(CFLAGS) $(NAME).o -O3 -o $(NAME)
