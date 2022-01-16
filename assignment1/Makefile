# Make File for Assignment 1. This Makefile is based upon the swarthmore
# makefile tutorial
# Name: Pranav Rajan
# Class: OS Spring 2022, Professor Ryan Stutsman
# Version: January 16, 2022

# Define Compiler and flags
CC = gcc
CFLAGS = -g -Wall

default: main

main: main.o assignment1.o
	$(CC) $(CFLAGS) -o main main.o assignment1.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

assignment1.o: assignment1.c assignment1.h
	$(CC) $(CFLAGS) -c assignment1.c

clean:
	$(RM) main *.o *~





