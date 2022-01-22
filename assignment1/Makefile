# Make File for Assignment 1. This Makefile is based upon the swarthmore
# makefile tutorial
# Name: Pranav Rajan
# Class: OS Spring 2022, Professor Ryan Stutsman
# Version: January 16, 2022

# Define Compiler and flags
# CC = gcc
# CFLAGS = -g -Wall

# default: main

# main: main.o assignment1.o
# 	$(CC) $(CFLAGS) -o main main.o assignment1.o

# main.o: main.c
# 	$(CC) $(CFLAGS) -c main.c

# assignment1.o: assignment1.c assignment1.h
# 	$(CC) $(CFLAGS) -c assignment1.c

# clean:
# 	$(RM) main *.o *~

assignment1.o: assignment1.c assignment1.h
	gcc -ggdb -O2 -std=c11 -Wall -Werror -pedantic -fsanitize=address -fsanitize=leak -fsanitize=undefined -fno-sanitize-recover -c assignment1.c -o assignment1.o

main: main.c assignment1.o
	gcc -ggdb -O2 -std=c11 -Wall -Werror -pedantic -fsanitize=address -fsanitize=leak -fsanitize=undefined -fno-sanitize-recover -o main main.c assignment1.o

test: main
	./main

clean:
	rm -rf main *.o u.txt

