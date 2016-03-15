# A basic makefile for "callsystem"
# written by hexu 
# 2009-1-8

CC = gcc
LDFLAGS = -lm
CFLAGS = -Wall

#OBJ = callsystem.o prtExitStat.o systemdemo.o 

#callsystem : $(OBJ)
prog10_20 : prog10_20.o my_system.o
	$(CC) $(LDFLAGS) -o $@ $^

prog10_20.o : prog10_20.c
	$(CC) $(CFLAGS) -o $@ -c $<

my_system.o : my_system.c
	$(CC) $(CFLAGS) -o $@ -c $<

