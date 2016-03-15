# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG
INCLUDE = 
LDFLAGS =
LDPATH = 

PROGS =	client_main server_main

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)


client_main:	client_main.o client.o mesg_recv.o mesg_send.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

server_main:	server_main.o server.o mesg_recv.o mesg_send.o sigchldwaitpid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

