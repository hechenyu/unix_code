# A basic makefile for "callsystem"
# written by hexu 
# 2009-1-8

CC = gcc
LDFLAGS = -lm
CFLAGS = -Wall -std=c89

#OBJ = callsystem.o prtExitStat.o systemdemo.o 

#callsystem : $(OBJ)
callsystem : callsystem.o prtExitStat.o systemdemo.o
	$(CC) $(LDFLAGS) -o $@ $^

callsystem.o : callsystem.c
	$(CC) $(CFLAGS) -o $@ -c $<

prtExitStat.o : prtExitStat.c
	$(CC) $(CFLAGS) -o $@ -c $<

systemdemo.o : systemdemo.c
	$(CC) $(CFLAGS) -o $@ -c $<
