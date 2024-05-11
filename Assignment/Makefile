#
# OS/Makefile
# Makefile for building the Sudoku Solution Validator
# Created by RyanM on 30/04/2024.
#

CC = gcc
CFLAGS = -pthread -Wall

mssv: mssv.o file_reader.o thread_functions.o utility.o thread_mgmt.o
    $(CC) $(CFLAGS) -o mssv mssv.o file_reader.o thread_functions.o utility.o thread_mgmt.o

mssv.o: mssv.c mssv.h
    $(CC) $(CFLAGS) -c mssv.c

file_reader.o: file_reader.c file_reader.h
    $(CC) $(CFLAGS) -c file_reader.c

thread_functions.o: thread_functions.c thread_functions.h
    $(CC) $(CFLAGS) -c thread_functions.c

utility.o: utility.c utility.h
    $(CC) $(CFLAGS) -c utility.c

thread_mgmt.o: thread_mgmt.c thread_mgmt.h
    $(CC) $(CFLAGS) -c thread_mgmt.c

clean:
    rm -f *.o mssv
