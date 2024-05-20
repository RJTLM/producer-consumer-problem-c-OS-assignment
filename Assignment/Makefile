#
# OS/Makefile
# Makefile for building the Sudoku Solution Validator
# Created by RyanM on 30/04/2024.
#

CC = gcc
CFLAGS = -Wall -pthread -g
SRCS = mssv.c sudoku_solution_file_reader.c sudoku_setup.c
OBJS = $(SRCS:.c=.o)

EXEC = mssv

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)
