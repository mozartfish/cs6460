# Make File for Assignment 1
# Name: Pranav Rajan
# Class: OS Spring 2022, Professor Ryan Stutsman
# Version: January 16, 2022

# Define Compiler and flags
CC = gcc
CFLAGS = -Wall 

OBJS = assignment1.o

all: assignment1

assignment1: $(OBJS)
            $(CC) $(CFLAGS) -o assignment1 $(OBJS) -lm

assignment1.o: assignment1.c assignment1.h

clean:
	rm -f *~ *.o assignment1


