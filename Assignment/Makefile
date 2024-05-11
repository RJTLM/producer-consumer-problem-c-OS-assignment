#
# OS/Makefile
# Makefile for building the Sudoku Solution Validator
# Created by RyanM on 30/04/2024.
#

CC=gcc
CFLAGS=-I./Assignment -Wall -g
LDFLAGS=-pthread

SRCS=$(wildcard Assignment/*.c)
OBJS=$(SRCS:.c=.o)
EXEC=mssv

all: $(EXEC)

$(EXEC): $(OBJS)
    $(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

clean:
    rm -f $(OBJS) $(EXEC)

.PHONY: all clean
